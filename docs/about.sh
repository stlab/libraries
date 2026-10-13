#!/bin/bash
set -euo pipefail

cd "$(dirname "${BASH_SOURCE[0]}")"

mkdir -p _data

cd _data

curl_options=(--fail --silent --show-error --location)
if [[ -n "${GITHUB_TOKEN:-}" ]]; then
    curl_options+=(--header "Authorization: Bearer ${GITHUB_TOKEN}")
fi

# Stage all responses so a failed request leaves the existing data intact.
temp_dir="$(mktemp -d .refresh.XXXXXX)"
trap 'rm -f "${temp_dir}/contributors.json" "${temp_dir}/releases.json" "${temp_dir}/tags.json"; rmdir "${temp_dir}"' EXIT

for endpoint in contributors releases tags; do
    curl "${curl_options[@]}" "https://api.github.com/repos/stlab/stlab/${endpoint}" \
        -o "${temp_dir}/${endpoint}.json"
done

mv "${temp_dir}/"*.json .
