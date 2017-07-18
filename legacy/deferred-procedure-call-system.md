---
title: Deferred Procedure Call System
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Deferred_Procedure_Call_System
---

#### Contents
{:.no_toc}
1. contents
{:toc}

## Overview

The DPCS is intended to queue up routine calls for execution at a later point in time. Such instances where this might be useful would be:
* Performing a task at a later time (preferrably at the unwinding of the call stack)
* "Throwing" errors across DLL boundaries
* Collecting various events for a set of windows (e.g., redraw or update events) then executing all the events for all the windows at a single time

## Terminology

verb
: A singleton procedure that is called at a later date.

verb_queue
: A collection of verbs.

behavior
: A collection of verbs, verb queues, and other behaviors (subbehaviors). As verbs, verb queues, and subbehaviors are inserted into a behavior, the order in which they were inserted is retained, and becomes the order in which the behavior executes inserted children.

## API

The current proposed API is as follows:

<pre>
typedef boost::function<void ()> verb_t;
</pre>

* A <code>verb_t</code> is a self-contained callback proc that is executed at a specific time

<pre>
typedef struct verb_queue_t* verb_q_token_t;
</pre>

* A <code>verb_queue_t</code> is a collection of verbs

<pre>
typedef struct behavior_t* behavior_token_t;
</pre>

* A <code>behavior_token_t</code> is a collection of verbs, verb queues, and other behaviors (subbehaviors)

<pre>
behavior_token_t make_behavior();
void destroy_behavior(behavior_token_t behavior);
</pre>

* <code>make_behavior</code> allocates a new behavior and returns the token to the client. The client is responsible for destroying the behavior with:
* <code>destroy_behavior</code> destroys memory allocated for the the behavior. If the behavior is a subbehavior within another behavior, it is detached from its parent behavior. Likewise if the behavior being destructed contains any subbehaviors, they are destroyed as well.

<pre>
void insert(behavior_token_t behavior, verb_t verb, bool single_execution);
</pre>

* This version of <code>insert</code> will add a single verb at this current point in the behavior. if <code>single_execution</code> is true then the verb will be destroyed after the first time it is executed.

<pre>
behavior_token_t insert_behavior(behavior_token_t behavior);
</pre>

* <code>insert_behavior</code> adds a subbehavior at this current point in the behavior. The function returns to the client the <code>behavior_token_t</code> of the newly created subbehavior. You are <i>not</i> responsible for destroying the subbehavior, though you may if you choose to do so (detaching it from the parent behavior).

<pre>
verb_q_token_t insert_queue(behavior_token_t behavior, bool single_execution);
</pre>

* <code>insert_queue</code> adds a verb queue at this current point in the behavior. The function returns to the client the <code>verb_q_token_t</code> of the newly created verb queue. You are not allowed to destroy verb queues. If <code>single_execution</code> is set to true, every verb in the queue will be destroyed after the first time it is. However, the verb queue itself will <i>not</i> be destroyed, allowing for further verb insertion at a later point into the same queue.

<pre>
bool empty(behavior_token_t behavior);
std::size_t size(behavior_token_t behavior);
</pre>

* <code>empty</code> returns to the client whether or not the behavior is empty.
* <code>size</code> returns the number of singleton verbs, verb queues, and subbehaviors that have been inserted into this behavior.

<pre>
void execute(behavior_token_t behavior);
</pre>

* <code>execute</code> will iterate through the children of this behavior and execute them in the order in which they were inserted. Note that this ordering is "one level deep" -- inserting a verb into a verb_queue that was inserted first into this behavior will result in that verb executing before any of the other immediate children of this behavior. If any verbs are marked as <code>single_execution</code>, they will not exist before this call is complete. Likewise, any verb queues marked <code>single_execution</code> will be empty before the completion of this function call.

<pre>
void insert(verb_q_token_t verb_q, verb_t verb);
</pre>

* This version of <code>insert</code> will append a verb to the end of this verb queue.

<pre>
bool empty(verb_q_token_t verb_q);
std::size_t size(verb_q_token_t verb_q);
</pre>

* <code>empty</code> returns to the client whether or not the verb queue is empty.
* <code>size</code> returns the number of singleton verbs that have been inserted into this verb queue.

<pre>
void execute(verb_q_token_t verb_q);
</pre>

* <code>execute</code> will iterate through the verbs of this verb queue and execute them in the order in which they were inserted into this queue. If this verb queue was marked <code>single_execution</code> it will be empty before the completion of this function call.

```
template<>
struct delete_ptr<behavior_token_t>
{
    typedef void result_type;

    void operator()(behavior_token_t x) const
        { if (x) destroy_behavior(x); }
};
```

* This template specialization is intended for use with the <code>adobe::auto_resource</code> class.

<pre>
verb_q_token_t general_deferred_proc_queue();
</pre>

* The general deferred proc queue is a catchall verb queue for verbs that need to be executed at the end of this iteration through the application's event loop. It is constructed as a <code>single_execution</code> verb queue. The application binary is required to execute this verb queue at the end of every iteration of its event loop.

## Issues

* There is a need for better locality - these are all things that have to happen after something else - the problem is the "something else" should not be known locally to the code doing the queuing.
* The basic need is for an explicit set of connections made by whatever code is connecting things up
