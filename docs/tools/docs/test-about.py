#!/usr/bin/env python3
"""Regression checks for the GitHub documentation data refresh (requires Bash)."""

import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


class RefreshDataTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.docs = self.root / "docs"
        self.data = self.docs / "_data"
        self.data.mkdir(parents=True)
        shutil.copyfile(Path(__file__).resolve().parents[2] / "about.sh",
                        self.docs / "about.sh")
        self.original = '[{"original": true}]\n'
        for endpoint in ("contributors", "releases", "tags"):
            (self.data / f"{endpoint}.json").write_text(self.original)
        self.bin = self.root / "bin"
        self.bin.mkdir()
        curl = self.bin / "curl"
        curl.write_text("""#!/usr/bin/env python3
import json
import os
from pathlib import Path
import sys

args = sys.argv[1:]
with open(os.environ["CURL_CALLS"], "a") as calls:
    calls.write(json.dumps(args) + "\\n")
endpoint = next(arg.rsplit("/", 1)[-1] for arg in args
                if arg.startswith("https://api.github.com/"))
output = Path(args[args.index("-o") + 1])
if endpoint == os.environ.get("FAIL_ENDPOINT"):
    output.write_text('{"message": "API rate limit exceeded"}')
    if "--fail" in args or "-f" in args:
        print("curl: (22) The requested URL returned error: 403", file=sys.stderr)
        sys.exit(22)
else:
    output.write_text(json.dumps([{"endpoint": endpoint}]))
""")
        curl.chmod(0o755)
        self.env = os.environ.copy()
        self.env.pop("GITHUB_TOKEN", None)
        self.env.update(PATH=f"{self.bin}{os.pathsep}{self.env['PATH']}",
                        CURL_CALLS=str(self.root / "calls"))

    def refresh(self):
        return subprocess.run(["bash", str(self.docs / "about.sh")],
                              env=self.env, capture_output=True, text=True)

    def test_http_error_stops_refresh_and_preserves_all_data(self):
        self.env["FAIL_ENDPOINT"] = "releases"
        result = self.refresh()
        self.assertNotEqual(result.returncode, 0, result.stderr)
        self.assertIn("403", result.stderr)
        for endpoint in ("contributors", "releases", "tags"):
            self.assertEqual((self.data / f"{endpoint}.json").read_text(),
                             self.original)
        self.assertEqual(sorted(path.name for path in self.data.iterdir()),
                         ["contributors.json", "releases.json", "tags.json"])

    def test_success_refreshes_all_endpoints_without_a_token(self):
        result = self.refresh()
        self.assertEqual(result.returncode, 0, result.stderr)
        for endpoint in ("contributors", "releases", "tags"):
            self.assertEqual(json.loads((self.data / f"{endpoint}.json").read_text()),
                             [{"endpoint": endpoint}])
        self.assertNotIn("Authorization", (self.root / "calls").read_text())

    def test_token_authenticates_every_request_without_being_logged(self):
        self.env["GITHUB_TOKEN"] = "test-token-not-a-secret"
        result = self.refresh()
        self.assertEqual(result.returncode, 0, result.stderr)
        calls = [json.loads(line) for line in (self.root / "calls").read_text().splitlines()]
        self.assertEqual(len(calls), 3)
        for args in calls:
            self.assertIn("Authorization: Bearer test-token-not-a-secret", args)
        self.assertNotIn(self.env["GITHUB_TOKEN"], result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main()
