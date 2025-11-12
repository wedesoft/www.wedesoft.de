---
layout: post
title: Clojure in your browser
category: software
image: /pics/clojure.png
---

There is a recent article on Clojure Civitas on [using Scittle for browser native slides][2].
[Scittle][1] is a Clojure interpreter that runs in the browser.
It even defines a script tag that let's you embed Clojure code in your HTML code.
Here is an example evaluating the content of an HTML textarea:

## HTML code

{% highlight html %}
<script src="https://cdn.jsdelivr.net/npm/scittle@0.6.22/dist/scittle.js"></script>
<script type="application/x-scittle">
(defn run []
  (let [code (.-value (js/document.getElementById "code"))
        output-elem (js/document.getElementById "output")]
    (try
      (let [result (.eval_string js/scittle.core code)]
        (set! (.-textContent output-elem) (str result)))
      (catch :default e
        (set! (.-textContent output-elem)
              (str "Error: " (.-message e)))))))

(set! (.-run js/window) run)
</script>
<textarea id="code" rows="20" style="width:100%;">
(def primes
  (lazy-seq
    (filter (fn [i] (not-any? #(zero? (rem i %))
                              (take-while #(<= (* % %) i) primes)))
            (drop 2 (range)))))
(take 100 primes)
</textarea>
<br />
<button id="run-button" onclick="run()">Run</button>
<pre id="output"></pre>
{% endhighlight %}

## Scittle in your browser

<script src="https://cdn.jsdelivr.net/npm/scittle@0.6.22/dist/scittle.js"></script>
<script type="application/x-scittle">
(defn run []
  (let [code (.-value (js/document.getElementById "code"))
        output-elem (js/document.getElementById "output")]
    (try
      (let [result (.eval_string js/scittle.core code)]
        (set! (.-textContent output-elem) (str result)))
      (catch :default e
        (set! (.-textContent output-elem)
              (str "Error: " (.-message e)))))))

(set! (.-run js/window) run)
</script>
<textarea id="code" rows="20" style="width:100%;">
(def primes
  (lazy-seq
    (filter (fn [i] (not-any? #(zero? (rem i %))
                              (take-while #(<= (* % %) i) primes)))
            (drop 2 (range)))))
(take 100 primes)
</textarea>
<br />
<button id="run-button" onclick="run()">Run</button>
<pre id="output"></pre>

[1]: https://github.com/babashka/scittle
[2]: https://clojurecivitas.github.io/scittle/presentations/browser_native_slides.html
