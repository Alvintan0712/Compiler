var _analytics = (function (exports) {
  'use strict';

  function _defineProperty(obj, key, value) {
    if (key in obj) {
      Object.defineProperty(obj, key, {
        value: value,
        enumerable: true,
        configurable: true,
        writable: true
      });
    } else {
      obj[key] = value;
    }

    return obj;
  }

  var defineProperty = _defineProperty;

  function ownKeys(object, enumerableOnly) {
    var keys = Object.keys(object);

    if (Object.getOwnPropertySymbols) {
      var symbols = Object.getOwnPropertySymbols(object);
      if (enumerableOnly) symbols = symbols.filter(function (sym) {
        return Object.getOwnPropertyDescriptor(object, sym).enumerable;
      });
      keys.push.apply(keys, symbols);
    }

    return keys;
  }

  function _objectSpread2(target) {
    for (var i = 1; i < arguments.length; i++) {
      var source = arguments[i] != null ? arguments[i] : {};

      if (i % 2) {
        ownKeys(source, true).forEach(function (key) {
          defineProperty(target, key, source[key]);
        });
      } else if (Object.getOwnPropertyDescriptors) {
        Object.defineProperties(target, Object.getOwnPropertyDescriptors(source));
      } else {
        ownKeys(source).forEach(function (key) {
          Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
        });
      }
    }

    return target;
  }

  var objectSpread2 = _objectSpread2;

  function dotProp (t, e, l, n, r) {
    for (e = e.split ? e.split(".") : e, n = 0; n < e.length; n++) t = t ? t[e[n]] : r;

    return t === r ? l : t;
  }

  var c = "undefined" != typeof window;

  function p(t) {
    return "function" == typeof t;
  }

  function a(t) {
    return "string" == typeof t;
  }

  function l(t) {
    return void 0 === t;
  }

  function d(t) {
    return "boolean" == typeof t;
  }

  function b(t) {
    return "[object Array]" === Object.prototype.toString.call(t);
  }

  function g(t) {
    if ("object" != typeof t || null === t) return !1;

    for (var n = t; null !== Object.getPrototypeOf(n);) n = Object.getPrototypeOf(n);

    return Object.getPrototypeOf(t) === n;
  }

  function n$1(e$$1) {
    try {
      return decodeURIComponent(e$$1.replace(/\+/g, " "));
    } catch (e$$1) {
      return null;
    }
  }

  function s$1(r$$1) {
    return function (e$$1) {
      for (var r$$1, t$$1 = Object.create(null), o$$1 = /([^&=]+)=?([^&]*)/g; r$$1 = o$$1.exec(e$$1);) {
        var a$$1 = n$1(r$$1[1]),
            i$$1 = n$1(r$$1[2]);
        "[]" === a$$1.substring(a$$1.length - 2) ? (t$$1[a$$1 = a$$1.substring(0, a$$1.length - 2)] || (t$$1[a$$1] = [])).push(i$$1) : t$$1[a$$1] = "" === i$$1 || i$$1;
      }

      for (var u$$1 in t$$1) {
        var c$$1 = u$$1.split("[");
        c$$1.length > 1 && (m$1(t$$1, c$$1.map(function (e$$1) {
          return e$$1.replace(/[?[\]\\ ]/g, "");
        }), t$$1[u$$1]), delete t$$1[u$$1]);
      }

      return t$$1;
    }(function (r$$1) {
      if (r$$1) {
        var t$$1 = r$$1.match(/\?(.*)/);
        return t$$1 && t$$1[1] ? t$$1[1].split("#")[0] : "";
      }

      return c && window.location.search.substring(1);
    }(r$$1));
  }

  function m$1(e$$1, r$$1, t$$1) {
    for (var n$$1 = r$$1.length - 1, o$$1 = 0; o$$1 < n$$1; ++o$$1) {
      var a$$1 = r$$1[o$$1];
      if ("__proto__" === a$$1 || "constructor" === a$$1) break;
      a$$1 in e$$1 || (e$$1[a$$1] = {}), e$$1 = e$$1[a$$1];
    }

    e$$1[r$$1[n$$1]] = t$$1;
  }

  function y$1() {
    for (var e$$1 = "", r$$1 = 0, t$$1 = 4294967295 * Math.random() | 0; r$$1++ < 36;) {
      var n$$1 = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx"[r$$1 - 1],
          o$$1 = 15 & t$$1;
      e$$1 += "-" == n$$1 || "4" == n$$1 ? n$$1 : ("x" == n$$1 ? o$$1 : 3 & o$$1 | 8).toString(16), t$$1 = r$$1 % 8 == 0 ? 4294967295 * Math.random() | 0 : t$$1 >> 4;
    }

    return e$$1;
  }

  var l$2 = "object" == typeof self && self.self === self && self || "object" == typeof global && global.global === global && global || void 0,
      e$1 = "undefined";

  function o$2(e) {
    return l$2[e];
  }

  function f$2(e, o) {
    return l$2[e] = o, o;
  }

  function n$2(l) {
    f$2(l);
  }

  function _arrayWithoutHoles(arr) {
    if (Array.isArray(arr)) {
      for (var i$$1 = 0, arr2 = new Array(arr.length); i$$1 < arr.length; i$$1++) {
        arr2[i$$1] = arr[i$$1];
      }

      return arr2;
    }
  }

  var arrayWithoutHoles = _arrayWithoutHoles;

  function _iterableToArray(iter) {
    if (Symbol.iterator in Object(iter) || Object.prototype.toString.call(iter) === "[object Arguments]") return Array.from(iter);
  }

  var iterableToArray = _iterableToArray;

  function _nonIterableSpread() {
    throw new TypeError("Invalid attempt to spread non-iterable instance");
  }

  var nonIterableSpread = _nonIterableSpread;

  function _toConsumableArray(arr) {
    return arrayWithoutHoles(arr) || iterableToArray(arr) || nonIterableSpread();
  }

  var toConsumableArray = _toConsumableArray;

  function createCommonjsModule(fn, module) {
    return module = {
      exports: {}
    }, fn(module, module.exports), module.exports;
  }

  var _typeof_1 = createCommonjsModule(function (module) {
    function _typeof2(obj) {
      if (typeof Symbol === "function" && typeof Symbol.iterator === "symbol") {
        _typeof2 = function _typeof2(obj) {
          return typeof obj;
        };
      } else {
        _typeof2 = function _typeof2(obj) {
          return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj;
        };
      }

      return _typeof2(obj);
    }

    function _typeof(obj) {
      if (typeof Symbol === "function" && _typeof2(Symbol.iterator) === "symbol") {
        module.exports = _typeof = function _typeof(obj) {
          return _typeof2(obj);
        };
      } else {
        module.exports = _typeof = function _typeof(obj) {
          return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : _typeof2(obj);
        };
      }

      return _typeof(obj);
    }

    module.exports = _typeof;
  });

  var runtime_1 = createCommonjsModule(function (module) {
    /**
     * Copyright (c) 2014-present, Facebook, Inc.
     *
     * This source code is licensed under the MIT license found in the
     * LICENSE file in the root directory of this source tree.
     */
    var runtime = function (exports) {
      var Op = Object.prototype;
      var hasOwn = Op.hasOwnProperty;
      var undefined; // More compressible than void 0.

      var $Symbol = typeof Symbol === "function" ? Symbol : {};
      var iteratorSymbol = $Symbol.iterator || "@@iterator";
      var asyncIteratorSymbol = $Symbol.asyncIterator || "@@asyncIterator";
      var toStringTagSymbol = $Symbol.toStringTag || "@@toStringTag";

      function define(obj, key, value) {
        Object.defineProperty(obj, key, {
          value: value,
          enumerable: true,
          configurable: true,
          writable: true
        });
        return obj[key];
      }

      try {
        // IE 8 has a broken Object.defineProperty that only works on DOM objects.
        define({}, "");
      } catch (err) {
        define = function (obj, key, value) {
          return obj[key] = value;
        };
      }

      function wrap(innerFn, outerFn, self, tryLocsList) {
        // If outerFn provided and outerFn.prototype is a Generator, then outerFn.prototype instanceof Generator.
        var protoGenerator = outerFn && outerFn.prototype instanceof Generator ? outerFn : Generator;
        var generator = Object.create(protoGenerator.prototype);
        var context = new Context(tryLocsList || []); // The ._invoke method unifies the implementations of the .next,
        // .throw, and .return methods.

        generator._invoke = makeInvokeMethod(innerFn, self, context);
        return generator;
      }

      exports.wrap = wrap; // Try/catch helper to minimize deoptimizations. Returns a completion
      // record like context.tryEntries[i].completion. This interface could
      // have been (and was previously) designed to take a closure to be
      // invoked without arguments, but in all the cases we care about we
      // already have an existing method we want to call, so there's no need
      // to create a new function object. We can even get away with assuming
      // the method takes exactly one argument, since that happens to be true
      // in every case, so we don't have to touch the arguments object. The
      // only additional allocation required is the completion record, which
      // has a stable shape and so hopefully should be cheap to allocate.

      function tryCatch(fn, obj, arg) {
        try {
          return {
            type: "normal",
            arg: fn.call(obj, arg)
          };
        } catch (err) {
          return {
            type: "throw",
            arg: err
          };
        }
      }

      var GenStateSuspendedStart = "suspendedStart";
      var GenStateSuspendedYield = "suspendedYield";
      var GenStateExecuting = "executing";
      var GenStateCompleted = "completed"; // Returning this object from the innerFn has the same effect as
      // breaking out of the dispatch switch statement.

      var ContinueSentinel = {}; // Dummy constructor functions that we use as the .constructor and
      // .constructor.prototype properties for functions that return Generator
      // objects. For full spec compliance, you may wish to configure your
      // minifier not to mangle the names of these two functions.

      function Generator() {}

      function GeneratorFunction() {}

      function GeneratorFunctionPrototype() {} // This is a polyfill for %IteratorPrototype% for environments that
      // don't natively support it.


      var IteratorPrototype = {};

      IteratorPrototype[iteratorSymbol] = function () {
        return this;
      };

      var getProto = Object.getPrototypeOf;
      var NativeIteratorPrototype = getProto && getProto(getProto(values([])));

      if (NativeIteratorPrototype && NativeIteratorPrototype !== Op && hasOwn.call(NativeIteratorPrototype, iteratorSymbol)) {
        // This environment has a native %IteratorPrototype%; use it instead
        // of the polyfill.
        IteratorPrototype = NativeIteratorPrototype;
      }

      var Gp = GeneratorFunctionPrototype.prototype = Generator.prototype = Object.create(IteratorPrototype);
      GeneratorFunction.prototype = Gp.constructor = GeneratorFunctionPrototype;
      GeneratorFunctionPrototype.constructor = GeneratorFunction;
      GeneratorFunction.displayName = define(GeneratorFunctionPrototype, toStringTagSymbol, "GeneratorFunction"); // Helper for defining the .next, .throw, and .return methods of the
      // Iterator interface in terms of a single ._invoke method.

      function defineIteratorMethods(prototype) {
        ["next", "throw", "return"].forEach(function (method) {
          define(prototype, method, function (arg) {
            return this._invoke(method, arg);
          });
        });
      }

      exports.isGeneratorFunction = function (genFun) {
        var ctor = typeof genFun === "function" && genFun.constructor;
        return ctor ? ctor === GeneratorFunction || // For the native GeneratorFunction constructor, the best we can
        // do is to check its .name property.
        (ctor.displayName || ctor.name) === "GeneratorFunction" : false;
      };

      exports.mark = function (genFun) {
        if (Object.setPrototypeOf) {
          Object.setPrototypeOf(genFun, GeneratorFunctionPrototype);
        } else {
          genFun.__proto__ = GeneratorFunctionPrototype;
          define(genFun, toStringTagSymbol, "GeneratorFunction");
        }

        genFun.prototype = Object.create(Gp);
        return genFun;
      }; // Within the body of any async function, `await x` is transformed to
      // `yield regeneratorRuntime.awrap(x)`, so that the runtime can test
      // `hasOwn.call(value, "__await")` to determine if the yielded value is
      // meant to be awaited.


      exports.awrap = function (arg) {
        return {
          __await: arg
        };
      };

      function AsyncIterator(generator, PromiseImpl) {
        function invoke(method, arg, resolve, reject) {
          var record = tryCatch(generator[method], generator, arg);

          if (record.type === "throw") {
            reject(record.arg);
          } else {
            var result = record.arg;
            var value = result.value;

            if (value && typeof value === "object" && hasOwn.call(value, "__await")) {
              return PromiseImpl.resolve(value.__await).then(function (value) {
                invoke("next", value, resolve, reject);
              }, function (err) {
                invoke("throw", err, resolve, reject);
              });
            }

            return PromiseImpl.resolve(value).then(function (unwrapped) {
              // When a yielded Promise is resolved, its final value becomes
              // the .value of the Promise<{value,done}> result for the
              // current iteration.
              result.value = unwrapped;
              resolve(result);
            }, function (error) {
              // If a rejected Promise was yielded, throw the rejection back
              // into the async generator function so it can be handled there.
              return invoke("throw", error, resolve, reject);
            });
          }
        }

        var previousPromise;

        function enqueue(method, arg) {
          function callInvokeWithMethodAndArg() {
            return new PromiseImpl(function (resolve, reject) {
              invoke(method, arg, resolve, reject);
            });
          }

          return previousPromise = // If enqueue has been called before, then we want to wait until
          // all previous Promises have been resolved before calling invoke,
          // so that results are always delivered in the correct order. If
          // enqueue has not been called before, then it is important to
          // call invoke immediately, without waiting on a callback to fire,
          // so that the async generator function has the opportunity to do
          // any necessary setup in a predictable way. This predictability
          // is why the Promise constructor synchronously invokes its
          // executor callback, and why async functions synchronously
          // execute code before the first await. Since we implement simple
          // async functions in terms of async generators, it is especially
          // important to get this right, even though it requires care.
          previousPromise ? previousPromise.then(callInvokeWithMethodAndArg, // Avoid propagating failures to Promises returned by later
          // invocations of the iterator.
          callInvokeWithMethodAndArg) : callInvokeWithMethodAndArg();
        } // Define the unified helper method that is used to implement .next,
        // .throw, and .return (see defineIteratorMethods).


        this._invoke = enqueue;
      }

      defineIteratorMethods(AsyncIterator.prototype);

      AsyncIterator.prototype[asyncIteratorSymbol] = function () {
        return this;
      };

      exports.AsyncIterator = AsyncIterator; // Note that simple async functions are implemented on top of
      // AsyncIterator objects; they just return a Promise for the value of
      // the final result produced by the iterator.

      exports.async = function (innerFn, outerFn, self, tryLocsList, PromiseImpl) {
        if (PromiseImpl === void 0) PromiseImpl = Promise;
        var iter = new AsyncIterator(wrap(innerFn, outerFn, self, tryLocsList), PromiseImpl);
        return exports.isGeneratorFunction(outerFn) ? iter // If outerFn is a generator, return the full iterator.
        : iter.next().then(function (result) {
          return result.done ? result.value : iter.next();
        });
      };

      function makeInvokeMethod(innerFn, self, context) {
        var state = GenStateSuspendedStart;
        return function invoke(method, arg) {
          if (state === GenStateExecuting) {
            throw new Error("Generator is already running");
          }

          if (state === GenStateCompleted) {
            if (method === "throw") {
              throw arg;
            } // Be forgiving, per 25.3.3.3.3 of the spec:
            // https://people.mozilla.org/~jorendorff/es6-draft.html#sec-generatorresume


            return doneResult();
          }

          context.method = method;
          context.arg = arg;

          while (true) {
            var delegate = context.delegate;

            if (delegate) {
              var delegateResult = maybeInvokeDelegate(delegate, context);

              if (delegateResult) {
                if (delegateResult === ContinueSentinel) continue;
                return delegateResult;
              }
            }

            if (context.method === "next") {
              // Setting context._sent for legacy support of Babel's
              // function.sent implementation.
              context.sent = context._sent = context.arg;
            } else if (context.method === "throw") {
              if (state === GenStateSuspendedStart) {
                state = GenStateCompleted;
                throw context.arg;
              }

              context.dispatchException(context.arg);
            } else if (context.method === "return") {
              context.abrupt("return", context.arg);
            }

            state = GenStateExecuting;
            var record = tryCatch(innerFn, self, context);

            if (record.type === "normal") {
              // If an exception is thrown from innerFn, we leave state ===
              // GenStateExecuting and loop back for another invocation.
              state = context.done ? GenStateCompleted : GenStateSuspendedYield;

              if (record.arg === ContinueSentinel) {
                continue;
              }

              return {
                value: record.arg,
                done: context.done
              };
            } else if (record.type === "throw") {
              state = GenStateCompleted; // Dispatch the exception by looping back around to the
              // context.dispatchException(context.arg) call above.

              context.method = "throw";
              context.arg = record.arg;
            }
          }
        };
      } // Call delegate.iterator[context.method](context.arg) and handle the
      // result, either by returning a { value, done } result from the
      // delegate iterator, or by modifying context.method and context.arg,
      // setting context.delegate to null, and returning the ContinueSentinel.


      function maybeInvokeDelegate(delegate, context) {
        var method = delegate.iterator[context.method];

        if (method === undefined) {
          // A .throw or .return when the delegate iterator has no .throw
          // method always terminates the yield* loop.
          context.delegate = null;

          if (context.method === "throw") {
            // Note: ["return"] must be used for ES3 parsing compatibility.
            if (delegate.iterator["return"]) {
              // If the delegate iterator has a return method, give it a
              // chance to clean up.
              context.method = "return";
              context.arg = undefined;
              maybeInvokeDelegate(delegate, context);

              if (context.method === "throw") {
                // If maybeInvokeDelegate(context) changed context.method from
                // "return" to "throw", let that override the TypeError below.
                return ContinueSentinel;
              }
            }

            context.method = "throw";
            context.arg = new TypeError("The iterator does not provide a 'throw' method");
          }

          return ContinueSentinel;
        }

        var record = tryCatch(method, delegate.iterator, context.arg);

        if (record.type === "throw") {
          context.method = "throw";
          context.arg = record.arg;
          context.delegate = null;
          return ContinueSentinel;
        }

        var info = record.arg;

        if (!info) {
          context.method = "throw";
          context.arg = new TypeError("iterator result is not an object");
          context.delegate = null;
          return ContinueSentinel;
        }

        if (info.done) {
          // Assign the result of the finished delegate to the temporary
          // variable specified by delegate.resultName (see delegateYield).
          context[delegate.resultName] = info.value; // Resume execution at the desired location (see delegateYield).

          context.next = delegate.nextLoc; // If context.method was "throw" but the delegate handled the
          // exception, let the outer generator proceed normally. If
          // context.method was "next", forget context.arg since it has been
          // "consumed" by the delegate iterator. If context.method was
          // "return", allow the original .return call to continue in the
          // outer generator.

          if (context.method !== "return") {
            context.method = "next";
            context.arg = undefined;
          }
        } else {
          // Re-yield the result returned by the delegate method.
          return info;
        } // The delegate iterator is finished, so forget it and continue with
        // the outer generator.


        context.delegate = null;
        return ContinueSentinel;
      } // Define Generator.prototype.{next,throw,return} in terms of the
      // unified ._invoke helper method.


      defineIteratorMethods(Gp);
      define(Gp, toStringTagSymbol, "Generator"); // A Generator should always return itself as the iterator object when the
      // @@iterator function is called on it. Some browsers' implementations of the
      // iterator prototype chain incorrectly implement this, causing the Generator
      // object to not be returned from this call. This ensures that doesn't happen.
      // See https://github.com/facebook/regenerator/issues/274 for more details.

      Gp[iteratorSymbol] = function () {
        return this;
      };

      Gp.toString = function () {
        return "[object Generator]";
      };

      function pushTryEntry(locs) {
        var entry = {
          tryLoc: locs[0]
        };

        if (1 in locs) {
          entry.catchLoc = locs[1];
        }

        if (2 in locs) {
          entry.finallyLoc = locs[2];
          entry.afterLoc = locs[3];
        }

        this.tryEntries.push(entry);
      }

      function resetTryEntry(entry) {
        var record = entry.completion || {};
        record.type = "normal";
        delete record.arg;
        entry.completion = record;
      }

      function Context(tryLocsList) {
        // The root entry object (effectively a try statement without a catch
        // or a finally block) gives us a place to store values thrown from
        // locations where there is no enclosing try statement.
        this.tryEntries = [{
          tryLoc: "root"
        }];
        tryLocsList.forEach(pushTryEntry, this);
        this.reset(true);
      }

      exports.keys = function (object) {
        var keys = [];

        for (var key in object) {
          keys.push(key);
        }

        keys.reverse(); // Rather than returning an object with a next method, we keep
        // things simple and return the next function itself.

        return function next() {
          while (keys.length) {
            var key = keys.pop();

            if (key in object) {
              next.value = key;
              next.done = false;
              return next;
            }
          } // To avoid creating an additional object, we just hang the .value
          // and .done properties off the next function object itself. This
          // also ensures that the minifier will not anonymize the function.


          next.done = true;
          return next;
        };
      };

      function values(iterable) {
        if (iterable) {
          var iteratorMethod = iterable[iteratorSymbol];

          if (iteratorMethod) {
            return iteratorMethod.call(iterable);
          }

          if (typeof iterable.next === "function") {
            return iterable;
          }

          if (!isNaN(iterable.length)) {
            var i$$1 = -1,
                next = function next() {
              while (++i$$1 < iterable.length) {
                if (hasOwn.call(iterable, i$$1)) {
                  next.value = iterable[i$$1];
                  next.done = false;
                  return next;
                }
              }

              next.value = undefined;
              next.done = true;
              return next;
            };

            return next.next = next;
          }
        } // Return an iterator with no values.


        return {
          next: doneResult
        };
      }

      exports.values = values;

      function doneResult() {
        return {
          value: undefined,
          done: true
        };
      }

      Context.prototype = {
        constructor: Context,
        reset: function (skipTempReset) {
          this.prev = 0;
          this.next = 0; // Resetting context._sent for legacy support of Babel's
          // function.sent implementation.

          this.sent = this._sent = undefined;
          this.done = false;
          this.delegate = null;
          this.method = "next";
          this.arg = undefined;
          this.tryEntries.forEach(resetTryEntry);

          if (!skipTempReset) {
            for (var name in this) {
              // Not sure about the optimal order of these conditions:
              if (name.charAt(0) === "t" && hasOwn.call(this, name) && !isNaN(+name.slice(1))) {
                this[name] = undefined;
              }
            }
          }
        },
        stop: function () {
          this.done = true;
          var rootEntry = this.tryEntries[0];
          var rootRecord = rootEntry.completion;

          if (rootRecord.type === "throw") {
            throw rootRecord.arg;
          }

          return this.rval;
        },
        dispatchException: function (exception) {
          if (this.done) {
            throw exception;
          }

          var context = this;

          function handle(loc, caught) {
            record.type = "throw";
            record.arg = exception;
            context.next = loc;

            if (caught) {
              // If the dispatched exception was caught by a catch block,
              // then let that catch block handle the exception normally.
              context.method = "next";
              context.arg = undefined;
            }

            return !!caught;
          }

          for (var i$$1 = this.tryEntries.length - 1; i$$1 >= 0; --i$$1) {
            var entry = this.tryEntries[i$$1];
            var record = entry.completion;

            if (entry.tryLoc === "root") {
              // Exception thrown outside of any try block that could handle
              // it, so set the completion value of the entire function to
              // throw the exception.
              return handle("end");
            }

            if (entry.tryLoc <= this.prev) {
              var hasCatch = hasOwn.call(entry, "catchLoc");
              var hasFinally = hasOwn.call(entry, "finallyLoc");

              if (hasCatch && hasFinally) {
                if (this.prev < entry.catchLoc) {
                  return handle(entry.catchLoc, true);
                } else if (this.prev < entry.finallyLoc) {
                  return handle(entry.finallyLoc);
                }
              } else if (hasCatch) {
                if (this.prev < entry.catchLoc) {
                  return handle(entry.catchLoc, true);
                }
              } else if (hasFinally) {
                if (this.prev < entry.finallyLoc) {
                  return handle(entry.finallyLoc);
                }
              } else {
                throw new Error("try statement without catch or finally");
              }
            }
          }
        },
        abrupt: function (type, arg) {
          for (var i$$1 = this.tryEntries.length - 1; i$$1 >= 0; --i$$1) {
            var entry = this.tryEntries[i$$1];

            if (entry.tryLoc <= this.prev && hasOwn.call(entry, "finallyLoc") && this.prev < entry.finallyLoc) {
              var finallyEntry = entry;
              break;
            }
          }

          if (finallyEntry && (type === "break" || type === "continue") && finallyEntry.tryLoc <= arg && arg <= finallyEntry.finallyLoc) {
            // Ignore the finally entry if control is not jumping to a
            // location outside the try/catch block.
            finallyEntry = null;
          }

          var record = finallyEntry ? finallyEntry.completion : {};
          record.type = type;
          record.arg = arg;

          if (finallyEntry) {
            this.method = "next";
            this.next = finallyEntry.finallyLoc;
            return ContinueSentinel;
          }

          return this.complete(record);
        },
        complete: function (record, afterLoc) {
          if (record.type === "throw") {
            throw record.arg;
          }

          if (record.type === "break" || record.type === "continue") {
            this.next = record.arg;
          } else if (record.type === "return") {
            this.rval = this.arg = record.arg;
            this.method = "return";
            this.next = "end";
          } else if (record.type === "normal" && afterLoc) {
            this.next = afterLoc;
          }

          return ContinueSentinel;
        },
        finish: function (finallyLoc) {
          for (var i$$1 = this.tryEntries.length - 1; i$$1 >= 0; --i$$1) {
            var entry = this.tryEntries[i$$1];

            if (entry.finallyLoc === finallyLoc) {
              this.complete(entry.completion, entry.afterLoc);
              resetTryEntry(entry);
              return ContinueSentinel;
            }
          }
        },
        "catch": function (tryLoc) {
          for (var i$$1 = this.tryEntries.length - 1; i$$1 >= 0; --i$$1) {
            var entry = this.tryEntries[i$$1];

            if (entry.tryLoc === tryLoc) {
              var record = entry.completion;

              if (record.type === "throw") {
                var thrown = record.arg;
                resetTryEntry(entry);
              }

              return thrown;
            }
          } // The context.catch method must only be called with a location
          // argument that corresponds to a known catch block.


          throw new Error("illegal catch attempt");
        },
        delegateYield: function (iterable, resultName, nextLoc) {
          this.delegate = {
            iterator: values(iterable),
            resultName: resultName,
            nextLoc: nextLoc
          };

          if (this.method === "next") {
            // Deliberately forget the last sent value so that we don't
            // accidentally pass it on to the delegate.
            this.arg = undefined;
          }

          return ContinueSentinel;
        }
      }; // Regardless of whether this script is executing as a CommonJS module
      // or not, return the runtime object so that we can declare the variable
      // regeneratorRuntime in the outer scope, which allows this module to be
      // injected easily by `bin/regenerator --include-runtime script.js`.

      return exports;
    }( // If this script is executing as a CommonJS module, use module.exports
    // as the regeneratorRuntime namespace. Otherwise create a new empty
    // object. Either way, the resulting object will be used to initialize
    // the regeneratorRuntime variable at the top of this file.
    module.exports);

    try {
      regeneratorRuntime = runtime;
    } catch (accidentalStrictMode) {
      // This module should not be running in strict mode, so the above
      // assignment should always work unless something is misconfigured. Just
      // in case runtime.js accidentally runs in strict mode, we can escape
      // strict mode using a global Function call. This could conceivably fail
      // if a Content Security Policy forbids using Function, but in that case
      // the proper solution is to fix the accidental strict mode problem. If
      // you've misconfigured your bundler to force strict mode and applied a
      // CSP to forbid Function, and you're not willing to fix either of those
      // problems, please detail your unique predicament in a GitHub issue.
      Function("r", "regeneratorRuntime = r")(runtime);
    }
  });
  var regenerator = runtime_1;

  function asyncGeneratorStep(gen, resolve, reject, _next, _throw, key, arg) {
    try {
      var info = gen[key](arg);
      var value = info.value;
    } catch (error) {
      reject(error);
      return;
    }

    if (info.done) {
      resolve(value);
    } else {
      Promise.resolve(value).then(_next, _throw);
    }
  }

  function _asyncToGenerator(fn) {
    return function () {
      var self = this,
          args = arguments;
      return new Promise(function (resolve, reject) {
        var gen = fn.apply(self, args);

        function _next(value) {
          asyncGeneratorStep(gen, resolve, reject, _next, _throw, "next", value);
        }

        function _throw(err) {
          asyncGeneratorStep(gen, resolve, reject, _next, _throw, "throw", err);
        }

        _next(undefined);
      });
    };
  }

  var asyncToGenerator = _asyncToGenerator;

  function _defineProperty$1(obj, key, value) {
    if (key in obj) {
      Object.defineProperty(obj, key, {
        value: value,
        enumerable: true,
        configurable: true,
        writable: true
      });
    } else {
      obj[key] = value;
    }

    return obj;
  }

  var defineProperty$1 = _defineProperty$1;

  function ownKeys$1(object, enumerableOnly) {
    var keys = Object.keys(object);

    if (Object.getOwnPropertySymbols) {
      var symbols = Object.getOwnPropertySymbols(object);
      if (enumerableOnly) symbols = symbols.filter(function (sym) {
        return Object.getOwnPropertyDescriptor(object, sym).enumerable;
      });
      keys.push.apply(keys, symbols);
    }

    return keys;
  }

  function _objectSpread2$1(target) {
    for (var i$$1 = 1; i$$1 < arguments.length; i$$1++) {
      var source = arguments[i$$1] != null ? arguments[i$$1] : {};

      if (i$$1 % 2) {
        ownKeys$1(source, true).forEach(function (key) {
          defineProperty$1(target, key, source[key]);
        });
      } else if (Object.getOwnPropertyDescriptors) {
        Object.defineProperties(target, Object.getOwnPropertyDescriptors(source));
      } else {
        ownKeys$1(source).forEach(function (key) {
          Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
        });
      }
    }

    return target;
  }

  var objectSpread2$1 = _objectSpread2$1;
  var FUNC = 'function';
  var UNDEF = 'undefined';
  var REDUCER = 'reducer';
  var base = '@@redux/';
  var ACTION_INIT = base + 'INIT';
  var ACTION_TEST = base + Math.random().toString(36);

  var $$observable =
  /* #__PURE__ */
  function () {
    return (typeof Symbol === "undefined" ? "undefined" : _typeof_1(Symbol)) === FUNC && Symbol.observable || '@@observable';
  }();
  /*
   * Creates a Redux store that holds the state tree.
   * The only way to change the data in the store is to call `dispatch()` on it.
   *
   * There should only be a single store in your app. To specify how different
   * parts of the state tree respond to actions, you may combine several reducers
   * into a single reducer function by using `combineReducers`.
   *
   * @param {Function} reducer A function that returns the next state tree, given
   * the current state tree and the action to handle.
   *
   * @param {any} [preloadedState] The initial state. You may optionally specify it
   * to hydrate the state from the server in universal apps, or to restore a
   * previously serialized user session.
   * If you use `combineReducers` to produce the root reducer function, this must be
   * an object with the same shape as `combineReducers` keys.
   *
   * @param {Function} [enhancer] The store enhancer. You may optionally specify it
   * to enhance the store with third-party capabilities such as middleware,
   * time travel, persistence, etc. The only store enhancer that ships with Redux
   * is `applyMiddleware()`.
   *
   * @returns {Store} A Redux store that lets you read the state, dispatch actions
   * and subscribe to changes.
   */


  var msg = ' != ' + FUNC;

  function createStore(reducer, preloadedState, enhancer) {
    if (_typeof_1(preloadedState) === FUNC && _typeof_1(enhancer) === UNDEF) {
      enhancer = preloadedState;
      preloadedState = undefined;
    }

    if (_typeof_1(enhancer) !== UNDEF) {
      if (_typeof_1(enhancer) !== FUNC) {
        throw new Error('enhancer' + msg);
      }

      return enhancer(createStore)(reducer, preloadedState);
    }

    if (_typeof_1(reducer) !== FUNC) {
      throw new Error(REDUCER + msg);
    }

    var currentReducer = reducer;
    var currentState = preloadedState;
    var currentListeners = [];
    var nextListeners = currentListeners;
    var isDispatching = false;

    function ensureCanMutateNextListeners() {
      if (nextListeners === currentListeners) {
        nextListeners = currentListeners.slice();
      }
    }
    /*
     * Reads the state tree managed by the store.
     *
     * @returns {any} The current state tree of your application.
     */


    function getState() {
      return currentState;
    }
    /*
     * Adds a change listener. It will be called any time an action is dispatched,
     * and some part of the state tree may potentially have changed. You may then
     * call `getState()` to read the current state tree inside the callback.
     *
     * You may call `dispatch()` from a change listener, with the following
     * caveats:
     *
     * 1. The subscriptions are snapshotted just before every `dispatch()` call.
     * If you subscribe or unsubscribe while the listeners are being invoked, this
     * will not have any effect on the `dispatch()` that is currently in progress.
     * However, the next `dispatch()` call, whether nested or not, will use a more
     * recent snapshot of the subscription list.
     *
     * 2. The listener should not expect to see all state changes, as the state
     * might have been updated multiple times during a nested `dispatch()` before
     * the listener is called. It is, however, guaranteed that all subscribers
     * registered before the `dispatch()` started will be called with the latest
     * state by the time it exits.
     *
     * @param {Function} listener A callback to be invoked on every dispatch.
     * @returns {Function} A function to remove this change listener.
     */


    function subscribe(listener) {
      if (_typeof_1(listener) !== FUNC) {
        throw new Error('Listener' + msg);
      }

      var isSubscribed = true;
      ensureCanMutateNextListeners();
      nextListeners.push(listener);
      return function unsubscribe() {
        if (!isSubscribed) {
          return;
        }

        isSubscribed = false;
        ensureCanMutateNextListeners();
        var index = nextListeners.indexOf(listener);
        nextListeners.splice(index, 1);
      };
    }
    /**
     * Dispatches an action. It is the only way to trigger a state change.
     *
     * The `reducer` function, used to create the store, will be called with the
     * current state tree and the given `action`. Its return value will
     * be considered the **next** state of the tree, and the change listeners
     * will be notified.
     *
     * The base implementation only supports plain object actions. If you want to
     * dispatch a Promise, an Observable, a thunk, or something else, you need to
     * wrap your store creating function into the corresponding middleware. For
     * example, see the documentation for the `redux-thunk` package. Even the
     * middleware will eventually dispatch plain object actions using this method.
     *
     * @param {Object} action A plain object representing “what changed”. It is
     * a good idea to keep actions serializable so you can record and replay user
     * sessions, or use the time travelling `redux-devtools`. An action must have
     * a `type` property which may not be `undefined`. It is a good idea to use
     * string constants for action types.
     *
     * @returns {Object} For convenience, the same action object you dispatched.
     *
     * Note that, if you use a custom middleware, it may wrap `dispatch()` to
     * return something else (for example, a Promise you can await).
     */


    function dispatch(action) {
      /* // add default info to actions... 
      console.log('dispatch before', _action)
      const action = {
        ..._action,
        ...{
          action: {
            ..._action.action,
            ...{ customInfo: 'yoooo'} 
          }
        }
      }
      console.log('dispatch after', action)
      /** */
      if (!g(action)) {
        throw new Error('Act != obj');
      }

      if (_typeof_1(action.type) === UNDEF) {
        throw new Error('ActType ' + UNDEF);
      }

      if (isDispatching) {
        throw new Error('Dispatch in ' + REDUCER);
      }

      try {
        isDispatching = true;
        currentState = currentReducer(currentState, action);
      } finally {
        isDispatching = false;
      }

      var listeners = currentListeners = nextListeners;

      for (var i$$1 = 0; i$$1 < listeners.length; i$$1++) {
        var listener = listeners[i$$1];
        listener();
      }

      return action;
    }
    /**
     * Replaces the reducer currently used by the store to calculate the state.
     *
     * You might need this if your app implements code splitting and you want to
     * load some of the reducers dynamically. You might also need this if you
     * implement a hot reloading mechanism for Redux.
     *
     * @param {Function} nextReducer The reducer for the store to use instead.
     * @returns {void}
     */


    function replaceReducer(nextReducer) {
      if (_typeof_1(nextReducer) !== FUNC) {
        throw new Error('next ' + REDUCER + msg);
      }

      currentReducer = nextReducer;
      dispatch({
        type: ACTION_INIT
      });
    }
    /**
     * Interoperability point for observable/reactive libraries.
     * @returns {observable} A minimal observable of state changes.
     * For more information, see the observable proposal:
     * https://github.com/tc39/proposal-observable
     */


    function observable() {
      var outerSubscribe = subscribe;
      return defineProperty$1({
        /*
         * The minimal observable subscription method.
         * @param {Object} observer Any object that can be used as an observer.
         * The observer object should have a `next` method.
         * @returns {subscription} An object with an `unsubscribe` method that can
         * be used to unsubscribe the observable from the store, and prevent further
         * emission of values from the observable.
         */
        subscribe: function subscribe(observer) {
          if (_typeof_1(observer) !== 'object') {
            throw new TypeError('Observer != obj');
          }

          function observeState() {
            if (observer.next) {
              observer.next(getState());
            }
          }

          observeState();
          var unsubscribe = outerSubscribe(observeState);
          return {
            unsubscribe: unsubscribe
          };
        }
      }, $$observable, function () {
        return this;
      });
    } // When a store is created, an "INIT" action is dispatched so that every
    // reducer returns their initial state. This effectively populates
    // the initial state tree.


    dispatch({
      type: ACTION_INIT
    });
    return defineProperty$1({
      dispatch: dispatch,
      subscribe: subscribe,
      getState: getState,
      replaceReducer: replaceReducer
    }, $$observable, observable);
  }

  function getUndefinedStateErrorMessage(key, action) {
    var actionType = action && action.type;
    var actionName = actionType && actionType.toString() || '?';
    return 'action ' + actionName + REDUCER + ' ' + key + ' returns ' + UNDEF;
  }

  function assertReducerShape(reducers) {
    Object.keys(reducers).forEach(function (key) {
      var reducer = reducers[key];
      var initialState = reducer(undefined, {
        type: ACTION_INIT
      });

      if (_typeof_1(initialState) === UNDEF || _typeof_1(reducer(undefined, {
        type: ACTION_TEST
      })) === UNDEF) {
        throw new Error(REDUCER + ' ' + key + ' ' + UNDEF);
      }
    });
  }
  /**
   * Turns an object whose values are different reducer functions, into a single
   * reducer function. It will call every child reducer, and gather their results
   * into a single state object, whose keys correspond to the keys of the passed
   * reducer functions.
   *
   * @param {Object} reducers An object whose values correspond to different
   * reducer functions that need to be combined into one. One handy way to obtain
   * it is to use ES6 `import * as reducers` syntax. The reducers may never return
   * undefined for any action. Instead, they should return their initial state
   * if the state passed to them was undefined, and the current state for any
   * unrecognized action.
   *
   * @returns {Function} A reducer function that invokes every reducer inside the
   * passed object, and builds a state object with the same shape.
   */


  function combineReducers(reducers) {
    var reducerKeys = Object.keys(reducers);
    var finalReducers = {};

    for (var i$$1 = 0; i$$1 < reducerKeys.length; i$$1++) {
      var key = reducerKeys[i$$1];

      if (_typeof_1(reducers[key]) === FUNC) {
        finalReducers[key] = reducers[key];
      }
    }

    var finalReducerKeys = Object.keys(finalReducers);
    var shapeAssertionError;

    try {
      assertReducerShape(finalReducers);
    } catch (e$$1) {
      shapeAssertionError = e$$1;
    }

    return function combination() {
      var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
      var action = arguments.length > 1 ? arguments[1] : undefined;

      if (shapeAssertionError) {
        throw shapeAssertionError;
      }

      var hasChanged = false;
      var nextState = {};

      for (var _i = 0; _i < finalReducerKeys.length; _i++) {
        var _key = finalReducerKeys[_i];
        var reducer = finalReducers[_key];
        var previousStateForKey = state[_key];
        var nextStateForKey = reducer(previousStateForKey, action);

        if (_typeof_1(nextStateForKey) === UNDEF) {
          var errorMessage = getUndefinedStateErrorMessage(_key, action);
          throw new Error(errorMessage);
        }

        nextState[_key] = nextStateForKey;
        hasChanged = hasChanged || nextStateForKey !== previousStateForKey;
      }

      return hasChanged ? nextState : state;
    };
  }

  function compose() {
    for (var _len = arguments.length, funcs = new Array(_len), _key = 0; _key < _len; _key++) {
      funcs[_key] = arguments[_key];
    }

    if (funcs.length === 0) {
      return function (arg) {
        return arg;
      };
    }

    if (funcs.length === 1) {
      return funcs[0];
    }

    return funcs.reduce(function (a$$1, b$$1) {
      return function () {
        return a$$1(b$$1.apply(void 0, arguments));
      };
    });
  }
  /**
   * Creates a store enhancer that applies middleware to the dispatch method
   * of the Redux store. This is handy for a variety of tasks, such as expressing
   * asynchronous actions in a concise manner, or logging every action payload.
   *
   * See `redux-thunk` package as an example of the Redux middleware.
   *
   * Because middleware is potentially asynchronous, this should be the first
   * store enhancer in the composition chain.
   *
   * Note that each middleware will be given the `dispatch` and `getState` functions
   * as named arguments.
   *
   * @param {...Function} middlewares The middleware chain to be applied.
   * @returns {Function} A store enhancer applying the middleware.
   */


  function applyMiddleware() {
    for (var _len = arguments.length, middlewares = new Array(_len), _key = 0; _key < _len; _key++) {
      middlewares[_key] = arguments[_key];
    }

    return function (createStore) {
      return function (reducer, preloadedState, enhancer) {
        var store = createStore(reducer, preloadedState, enhancer);
        var _dispatch = store.dispatch;
        var chain = [];
        var middlewareAPI = {
          getState: store.getState,
          dispatch: function dispatch(action) {
            return _dispatch(action);
          }
        };
        chain = middlewares.map(function (middleware) {
          return middleware(middlewareAPI);
        });
        _dispatch = compose.apply(void 0, toConsumableArray(chain))(store.dispatch);
        return objectSpread2$1(objectSpread2$1({}, store), {}, {
          dispatch: _dispatch
        });
      };
    };
  }

  var LIBRARY_NAME = 'analytics';
  var ID = 'userId';
  var ANONID = 'anonymousId';
  var ERROR_URL = 'https://lytics.dev/errors/';
  var PREFIX = '__';
  /**
   * Anonymous visitor Id localstorage key
   * @typedef {String} ANON_ID
   */

  var ANON_ID = PREFIX + 'anon_id'; // __anon_id

  /**
   * Visitor Id localstorage key
   * @typedef {String} USER_ID
   */

  var USER_ID = PREFIX + 'user_id'; // __user_id

  /**
   * Visitor traits localstorage key
   * @typedef {String} USER_TRAITS
   */

  var USER_TRAITS = PREFIX + 'user_traits'; // __user_traits

  var constants =
  /*#__PURE__*/
  Object.freeze({
    ANON_ID: ANON_ID,
    USER_ID: USER_ID,
    USER_TRAITS: USER_TRAITS
  });
  var coreEvents = [
  /**
   * `bootstrap` - Fires when analytics library starts up.
   * This is the first event fired. '.on/once' listeners are not allowed on bootstrap
   * Plugins can attach logic to this event
   */
  'bootstrap',
  /**
   * `params` - Fires when analytics parses URL parameters
   */
  'params',
  /**
   * `campaign` - Fires if params contain "utm" parameters
   */
  'campaign',
  /**
   * `initializeStart` - Fires before 'initialize', allows for plugins to cancel loading of other plugins
   */
  'initializeStart',
  /**
   * `initialize` - Fires when analytics loads plugins
   */
  'initialize',
  /**
   * `initializeEnd` - Fires after initialize, allows for plugins to run logic after initialization methods run
   */
  'initializeEnd',
  /**
   * `ready` - Fires when all analytic providers are fully loaded. This waits for 'initialize' and 'loaded' to return true
   */
  'ready',
  /**
   * `resetStart` - Fires if analytic.reset() is called.
   * Use this event to cancel reset based on a specific condition
   */
  'resetStart',
  /**
   * `reset` - Fires if analytic.reset() is called.
   * Use this event to run custom cleanup logic (if needed)
   */
  'reset',
  /**
   * `resetEnd` - Fires after analytic.reset() is called.
   * Use this event to run a callback after user data is reset
   */
  'resetEnd',
  /******************
   * Page Events
   ******************/

  /**
   * `pageStart` - Fires before 'page' events fire.
   *  This allows for dynamic page view cancellation based on current state of user or options passed in.
   */
  'pageStart',
  /**
   * `page` - Core analytics hook for page views.
   *  If your plugin or integration tracks page views, this is the event to fire on.
   */
  'page',
  /**
   * `pageEnd` - Fires after all registered 'page' methods fire.
   */
  'pageEnd',
  /**
   * `pageAborted` - Fires if 'page' call is cancelled by a plugin
   */
  'pageAborted',
  /****************
   * Track Events
   ***************/

  /**
   * `trackStart` - Called before the 'track' events fires.
   *  This allows for dynamic page view cancellation based on current state of user or options passed in.
   */
  'trackStart',
  /**
   * `track` - Core analytics hook for event tracking.
   *  If your plugin or integration tracks custom events, this is the event to fire on.
   */
  'track',
  /**
   * `trackEnd` - Fires after all registered 'track' events fire from plugins.
   */
  'trackEnd',
  /**
   * `trackAborted` - Fires if 'track' call is cancelled by a plugin
   */
  'trackAborted',
  /******************
   * Identify Events
   ******************/

  /**
   * `identifyStart` - Called before the 'identify' events fires.
   * This allows for dynamic page view cancellation based on current state of user or options passed in.
   */
  'identifyStart',
  /**
   * `identify` - Core analytics hook for user identification.
   *  If your plugin or integration identifies users or user traits, this is the event to fire on.
   */
  'identify',
  /**
   * `identifyEnd` - Fires after all registered 'identify' events fire from plugins.
   */
  'identifyEnd',
  /**
   * `identifyAborted` - Fires if 'track' call is cancelled by a plugin
   */
  'identifyAborted',
  /**
   * `userIdChanged` - Fires when a user id is updated
   */
  'userIdChanged',
  /******************
   * Plugin Events
   ******************/

  /**
   * `registerPlugins` - Fires when analytics is registering plugins
   */
  'registerPlugins',
  /**
   * `enablePlugin` - Fires when 'analytics.plugins.enable()' is called
   */
  'enablePlugin',
  /**
   * `disablePlugin` - Fires when 'analytics.plugins.disable()' is called
   */
  'disablePlugin',
  /*
   * `loadPlugin` - Fires when 'analytics.loadPlugin()' is called
   */
  // 'loadPlugin',

  /******************
   * Browser activity events
   ******************/

  /**
   * `online` - Fires when browser network goes online.
   * This fires only when coming back online from an offline state.
   */
  'online',
  /**
   * `offline` - Fires when browser network goes offline.
   */
  'offline',
  /******************
   * Storage events
   ******************/

  /**
   * `setItemStart` - Fires when analytics.storage.setItem is initialized.
   * This event gives plugins the ability to intercept keys & values and alter them before they are persisted.
   */
  'setItemStart',
  /**
   * `setItem` - Fires when analytics.storage.setItem is called.
   * This event gives plugins the ability to intercept keys & values and alter them before they are persisted.
   */
  'setItem',
  /**
   * `setItemEnd` - Fires when setItem storage is complete.
   */
  'setItemEnd',
  /**
   * `setItemAborted` - Fires when setItem storage is cancelled by a plugin.
   */
  'setItemAborted',
  /**
   * `removeItemStart` - Fires when analytics.storage.removeItem is initialized.
   * This event gives plugins the ability to intercept removeItem calls and abort / alter them.
   */
  'removeItemStart',
  /**
   * `removeItem` - Fires when analytics.storage.removeItem is called.
   * This event gives plugins the ability to intercept removeItem calls and abort / alter them.
   */
  'removeItem',
  /**
   * `removeItemEnd` - Fires when removeItem storage is complete.
   */
  'removeItemEnd',
  /**
   * `removeItemAborted` - Fires when removeItem storage is cancelled by a plugin.
   */
  'removeItemAborted'];
  /* Keys on a plugin that are not considered events */

  var nonEvents = ['name', 'EVENTS', 'config', 'loaded'];
  var pluginEvents = {
    registerPluginType: function registerPluginType(name) {
      return "registerPlugin:".concat(name);
    },
    pluginReadyType: function pluginReadyType(name) {
      return "ready:".concat(name);
    }
  };
  var EVENTS = coreEvents.reduce(function (acc, curr) {
    acc[curr] = curr;
    return acc;
  }, pluginEvents);

  function isReservedAction(type) {
    return coreEvents.includes(type);
  }

  var utmRegex = /^utm_/;
  var propRegex = /^an_prop_/;
  var traitRegex = /^an_trait_/; // Middleware runs during EVENTS.initialize

  function initializeMiddleware(instance) {
    var setItem = instance.storage.setItem;
    return function (store) {
      return function (next) {
        return function (action) {
          /* Handle bootstrap event */
          if (action.type === EVENTS.bootstrap) {
            var params = action.params,
                user = action.user,
                persistedUser = action.persistedUser,
                initialUser = action.initialUser;
            var isKnownId = persistedUser.userId === user.userId;
            /* 1. Set anonymous ID */

            if (persistedUser.anonymousId !== user.anonymousId) {
              setItem(ANON_ID, user.anonymousId);
            }
            /* 2. Set userId */


            if (!isKnownId) {
              setItem(USER_ID, user.userId);
            }
            /* 3. Set traits if they are different */


            if (initialUser.traits) {
              setItem(USER_TRAITS, objectSpread2$1(objectSpread2$1({}, isKnownId && persistedUser.traits ? persistedUser.traits : {}), initialUser.traits));
              /* TODO multi user setup
              setItem(`${USER_TRAITS}.${user.userId}`, {
                ...(isKnownId) ? existingTraits : {},
                ...initialUser.traits
              })
              */
            }
            /* 4. Parse url params */


            var paramsArray = Object.keys(action.params);

            if (paramsArray.length) {
              var an_uid = params.an_uid,
                  an_event = params.an_event;
              var groupedParams = paramsArray.reduce(function (acc, key) {
                // match utm params & dclid (display) & gclid (cpc)
                if (key.match(utmRegex) || key.match(/^(d|g)clid/)) {
                  var cleanName = key.replace(utmRegex, '');
                  var keyName = cleanName === 'campaign' ? 'name' : cleanName;
                  acc.campaign[keyName] = params[key];
                }

                if (key.match(propRegex)) {
                  acc.props[key.replace(propRegex, '')] = params[key];
                }

                if (key.match(traitRegex)) {
                  acc.traits[key.replace(traitRegex, '')] = params[key];
                }

                return acc;
              }, {
                campaign: {},
                props: {},
                traits: {}
              });
              store.dispatch(objectSpread2$1(objectSpread2$1({
                type: EVENTS.params,
                raw: params
              }, groupedParams), an_uid ? {
                userId: an_uid
              } : {}));
              /* If userId set, call identify */

              if (an_uid) {
                // timeout to debounce and make sure integration is registered. Todo refactor
                setTimeout(function () {
                  return instance.identify(an_uid, groupedParams.traits);
                }, 0);
              }
              /* If tracking event set, call track */


              if (an_event) {
                // timeout to debounce and make sure integration is registered. Todo refactor
                setTimeout(function () {
                  return instance.track(an_event, groupedParams.props);
                }, 0);
              } // if url has utm params


              if (Object.keys(groupedParams.campaign).length) {
                store.dispatch({
                  type: EVENTS.campaign,
                  campaign: groupedParams.campaign
                });
              }
            }
          }

          return next(action);
        };
      };
    };
  }
  /* user reducer */


  function userReducer(storage) {
    return function user() {
      var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
      var action = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};

      if (action.type === EVENTS.setItemEnd) {
        // Set anonymousId if changed by storage.setItem
        if (action.key === ANON_ID) {
          return objectSpread2$1(objectSpread2$1({}, state), {
            anonymousId: action.value
          });
        } // Set userId if changed by storage.setItem


        if (action.key === USER_ID) {
          return objectSpread2$1(objectSpread2$1({}, state), {
            userId: action.value
          });
        }
      }

      switch (action.type) {
        case EVENTS.identify:
          return Object.assign({}, state, {
            userId: action.userId,
            traits: objectSpread2$1(objectSpread2$1({}, state.traits), action.traits)
          });

        case EVENTS.reset:
          // Side effect to fix race condition in Node. TODO refactor
          // This is from default storage.removeItem: (key) => globalContext[key] = undefined
          [USER_ID, ANON_ID, USER_TRAITS].forEach(function (key) {
            // sync storage, not instance.storage
            storage.removeItem(key);
          });
          return Object.assign({}, state, {
            userId: null,
            // TODO reset anon id automatically?
            anonymousId: null,
            traits: {}
          });

        default:
          return state;
      }
    };
  }

  function getPersistedUserData(storage) {
    return {
      userId: storage.getItem(USER_ID),
      anonymousId: storage.getItem(ANON_ID),
      traits: storage.getItem(USER_TRAITS)
    };
  }

  var tempKey = function tempKey(key) {
    return PREFIX + 'TEMP' + PREFIX + key;
  };

  function getUserPropFunc(storage) {
    return function getUserProp(key, instance, payload) {
      /* 1. Try current state */
      var currentId = instance.getState('user')[key];

      if (currentId) {
        /*
        console.log(`from state ${key}`, currentId)
        /** */
        return currentId;
      }
      /* 2. Try event payload */


      if (payload && g(payload) && payload[key]) {
        /*
        console.log(`from payload ${key}`, payload[key])
        /** */
        return payload[key];
      }
      /* 3. Try persisted data */


      var persistedInfo = getPersistedUserData(storage)[key];

      if (persistedInfo) {
        /*
        console.log(`from persistedInfo ${key}`, persistedInfo)
        /** */
        return persistedInfo;
      }
      /* 4. Else, try to get in memory placeholder. TODO watch this for future issues */


      return o$2(tempKey(key)) || null;
    };
  }

  function identifyMiddleware(instance) {
    var _instance$storage = instance.storage,
        setItem = _instance$storage.setItem,
        removeItem = _instance$storage.removeItem,
        getItem = _instance$storage.getItem;
    return function (store) {
      return function (next) {
        return function (action) {
          var userId = action.userId,
              traits = action.traits,
              options = action.options;
          /* Reset user id and traits */

          if (action.type === EVENTS.reset) {
            // Remove stored data
            [USER_ID, USER_TRAITS, ANON_ID].forEach(function (key) {
              // Fires async removeItem dispatch
              removeItem(key);
            });
            [ID, ANONID, 'traits'].forEach(function (key) {
              // Remove from global context
              n$2(tempKey(key));
            });
          }

          if (action.type === EVENTS.identify) {
            /* If no anon id. Set it! */
            if (!getItem(ANON_ID)) {
              setItem(ANON_ID, y$1());
            }

            var currentId = getItem(USER_ID);
            var currentTraits = getItem(USER_TRAITS) || {};

            if (currentId && currentId !== userId) {
              store.dispatch({
                type: EVENTS.userIdChanged,
                old: {
                  userId: currentId,
                  traits: currentTraits
                },
                "new": {
                  userId: userId,
                  traits: traits
                },
                options: options
              });
            }
            /* Save user id */


            if (userId) {
              setItem(USER_ID, userId);
            }
            /* Save user traits */


            if (traits) {
              setItem(USER_TRAITS, objectSpread2$1(objectSpread2$1({}, currentTraits), traits));
            }
          }

          return next(action);
        };
      };
    };
  }

  var stack = {};

  function runCallback(id, payload) {
    if (stack[id] && p(stack[id])) {
      // console.log(`run ${id}`)
      stack[id](payload);
      delete stack[id];
    }
  }

  function waitForReady(data, predicate, timeout) {
    return new Promise(function (resolve, reject) {
      if (predicate()) {
        return resolve(data);
      } // Timeout. Add to queue


      if (timeout < 1) {
        return reject(objectSpread2$1(objectSpread2$1({}, data), {}, {
          queue: true
        })); // eslint-disable-line
      } // Else recursive retry


      return pause(10).then(function (_) {
        return waitForReady(data, predicate, timeout - 10).then(resolve, reject);
      });
    });
  }

  function pause(ms) {
    return new Promise(function (resolve) {
      return setTimeout(resolve, ms);
    });
  }

  function processQueue(store, getPlugins, instance) {
    var pluginMethods = getPlugins();

    var _store$getState = store.getState(),
        plugins = _store$getState.plugins,
        context = _store$getState.context,
        queue = _store$getState.queue,
        user = _store$getState.user;

    var isOnline = !context.offline;
    /* If network connection found and there is items in queue, process them all */

    if (isOnline && queue && queue.actions && queue.actions.length) {
      var pipeline = queue.actions.reduce(function (acc, item, index) {
        var isLoaded = plugins[item.plugin].loaded;

        if (isLoaded) {
          acc.process.push(item);
          acc.processIndex.push(index);
        } else {
          acc.requeue.push(item);
          acc.requeueIndex.push(index);
        }

        return acc;
      }, {
        processIndex: [],
        process: [],
        requeue: [],
        requeueIndex: []
      });

      if (pipeline.processIndex && pipeline.processIndex.length) {
        pipeline.processIndex.forEach(function (i$$1) {
          var processAction = queue.actions[i$$1]; // console.log('RePROCESS THIS>', processAction)
          // Call methods directly right now

          var currentPlugin = processAction.plugin;
          var currentMethod = processAction.payload.type;
          var method = pluginMethods[currentPlugin][currentMethod];

          if (method && p(method)) {
            /* enrich queued payload with userId / anon id if missing */

            /* TODO hoist enrich into where action queued? */
            // console.log('before', processAction.payload)
            var enrichedPayload = enrich(processAction.payload, user); // console.log('user.userId', user.userId)
            // console.log('user.anonymousId', user.anonymousId)
            // console.log('after enrich', enrichedPayload)

            method({
              payload: enrichedPayload,
              config: plugins[currentPlugin].config,
              instance: instance
            });
            /* Then redispatch for .on listeners / other middleware */

            var pluginEvent = "".concat(currentMethod, ":").concat(currentPlugin);
            store.dispatch(objectSpread2$1(objectSpread2$1({}, enrichedPayload), {}, {
              type: pluginEvent,

              /* Internal data for analytics engine */
              _: {
                called: pluginEvent,
                from: 'queueDrain'
              }
            }));
          }
        });
        /* Removed processed actions */

        var reQueueActions = queue.actions.filter(function (value, index) {
          // note !~ === return pipeline.processIndex.indexOf(index) === -1
          return !~pipeline.processIndex.indexOf(index);
        });
        /* Set queue actions. TODO refactor to non mutatable or move out of redux */

        queue.actions = reQueueActions;
      }
    }
  }

  function enrich() {
    var payload = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
    var user = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};
    return [ID, ANONID].reduce(function (acc, key) {
      if (payload.hasOwnProperty(key) && user[key] && user[key] !== payload[key]) {
        // console.log(`${key} stale update with ${user[key]}`)
        acc[key] = user[key];
      }

      return acc;
    }, payload);
  }

  function _arrayWithHoles(arr) {
    if (Array.isArray(arr)) return arr;
  }

  var arrayWithHoles = _arrayWithHoles;

  function _iterableToArrayLimit(arr, i$$1) {
    var _arr = [];
    var _n = true;
    var _d = false;
    var _e = undefined;

    try {
      for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) {
        _arr.push(_s.value);

        if (i$$1 && _arr.length === i$$1) break;
      }
    } catch (err) {
      _d = true;
      _e = err;
    } finally {
      try {
        if (!_n && _i["return"] != null) _i["return"]();
      } finally {
        if (_d) throw _e;
      }
    }

    return _arr;
  }

  var iterableToArrayLimit = _iterableToArrayLimit;

  function _nonIterableRest() {
    throw new TypeError("Invalid attempt to destructure non-iterable instance");
  }

  var nonIterableRest = _nonIterableRest;

  function _slicedToArray(arr, i$$1) {
    return arrayWithHoles(arr) || iterableToArrayLimit(arr, i$$1) || nonIterableRest();
  }

  var slicedToArray = _slicedToArray;

  function fitlerDisabledPlugins(allPlugins) {
    var settings = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};
    var options = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : {};
    return Object.keys(allPlugins).filter(function (name) {
      var fromCallOptions = options.plugins || {}; // If enabled/disabled by options. Override settings

      if (d(fromCallOptions[name])) {
        return fromCallOptions[name];
      } // If all: false disable everything unless true explicitly set


      if (fromCallOptions.all === false) {
        return false;
      } // else use state.plugin settings


      if (settings[name] && settings[name].enabled === false) {
        return false;
      }

      return true;
    }).map(function (name) {
      return allPlugins[name];
    });
  }

  var endsWithStartRegex = /Start$/;
  var bootstrapRegex = /^bootstrap/;
  var readyRegex = /^ready/;

  function runPlugins(_x, _x2, _x3, _x4, _x5) {
    return _ref.apply(this, arguments);
  }
  /**
   * Async reduce over matched plugin methods
   * Fires plugin functions
   */


  function _ref() {
    _ref = asyncToGenerator(
    /*#__PURE__*/
    regenerator.mark(function _callee(action, getPlugins, instance, store, eventsInfo) {
      var pluginObject, originalType, updatedType, state, activePlugins, allActivePluginKeys, allMatches, actionBefore, actionDuring, afterName, actionAfter;
      return regenerator.wrap(function _callee$(_context) {
        while (1) {
          switch (_context.prev = _context.next) {
            case 0:
              pluginObject = p(getPlugins) ? getPlugins() : getPlugins;
              originalType = action.type;
              updatedType = originalType.replace(endsWithStartRegex, '');
              /* If action already dispatched exit early. This makes it so plugin methods are not fired twice. */

              if (!(action._ && action._.called)) {
                _context.next = 5;
                break;
              }

              return _context.abrupt("return", action);

            case 5:
              state = instance.getState();
              /* Remove plugins that are disabled by options or by settings */

              activePlugins = fitlerDisabledPlugins(pluginObject, state.plugins, action.options);
              /* If analytics.plugin.enable calls do special behavior */

              if (originalType === EVENTS.initializeStart && action.fromEnable) {
                // Return list of all enabled plugins that have NOT been initialized yet
                activePlugins = Object.keys(state.plugins).filter(function (name) {
                  var info = state.plugins[name];
                  return action.plugins.includes(name) && !info.initialized;
                }).map(function (name) {
                  return pluginObject[name];
                });
              } // console.log(`engine activePlugins ${action.type}`, activePlugins)


              allActivePluginKeys = activePlugins.map(function (p$$1) {
                return p$$1.name;
              }); // console.log('allActivePluginKeys', allActivePluginKeys)

              allMatches = getAllMatchingCalls(originalType, activePlugins, pluginObject); // console.log('allMatches', allMatches)

              /* @TODO cache matches and purge on enable/disable/add plugin */

              /**
               * Process all 'actionBefore' hooks
               * Example:
               *  This is processes 'pageStart' methods from plugins and update the event to send through the chain
               */

              _context.next = 12;
              return processEvent({
                action: action,
                data: {
                  exact: allMatches.before,
                  namespaced: allMatches.beforeNS
                },
                state: state,
                allPlugins: pluginObject,
                allMatches: allMatches,
                instance: instance,
                store: store,
                EVENTS: eventsInfo
              });

            case 12:
              actionBefore = _context.sent;

              if (!shouldAbortAll(actionBefore, allActivePluginKeys.length)) {
                _context.next = 15;
                break;
              }

              return _context.abrupt("return", actionBefore);

            case 15:
              if (!(originalType === updatedType)) {
                _context.next = 19;
                break;
              }
              /* If type the same don't double process */


              actionDuring = actionBefore;
              _context.next = 22;
              break;

            case 19:
              _context.next = 21;
              return processEvent({
                action: objectSpread2$1(objectSpread2$1({}, actionBefore), {}, {
                  type: updatedType
                }),
                data: {
                  exact: allMatches.during,
                  namespaced: allMatches.duringNS
                },
                state: state,
                allPlugins: pluginObject,
                allMatches: allMatches,
                instance: instance,
                store: store,
                EVENTS: eventsInfo
              });

            case 21:
              actionDuring = _context.sent;

            case 22:
              if (!originalType.match(endsWithStartRegex)) {
                _context.next = 28;
                break;
              }

              afterName = "".concat(updatedType, "End");
              _context.next = 26;
              return processEvent({
                action: objectSpread2$1(objectSpread2$1({}, actionDuring), {}, {
                  type: afterName
                }),
                data: {
                  exact: allMatches.after,
                  namespaced: allMatches.afterNS
                },
                state: state,
                allPlugins: pluginObject,
                allMatches: allMatches,
                instance: instance,
                store: store,
                EVENTS: eventsInfo
              });

            case 26:
              actionAfter = _context.sent; // console.log('____ actionAfter', actionAfter)

              /* Fire callback if supplied */

              if (actionAfter.meta && actionAfter.meta.hasCallback) {
                /*
                console.log('End of engine action has callback')
                console.log(actionAfter.meta)
                console.log('stack', stack)
                /** */
                // @TODO figure out exact args calls and .on will get
                runCallback(actionAfter.meta.rid, {
                  payload: actionAfter
                });
              }

            case 28:
              return _context.abrupt("return", actionBefore);

            case 29:
            case "end":
              return _context.stop();
          }
        }
      }, _callee);
    }));
    return _ref.apply(this, arguments);
  }

  function processEvent(_x6) {
    return _processEvent.apply(this, arguments);
  }

  function _processEvent() {
    _processEvent = asyncToGenerator(
    /*#__PURE__*/
    regenerator.mark(function _callee5(_ref2) {
      var data, action, instance, state, allPlugins, allMatches, store, EVENTS$$1, plugins, context, method, isStartEvent, abortable, makeArgs, queueData, payloads, resolvedAction, endAction;
      return regenerator.wrap(function _callee5$(_context5) {
        while (1) {
          switch (_context5.prev = _context5.next) {
            case 0:
              data = _ref2.data, action = _ref2.action, instance = _ref2.instance, state = _ref2.state, allPlugins = _ref2.allPlugins, allMatches = _ref2.allMatches, store = _ref2.store, EVENTS$$1 = _ref2.EVENTS;
              plugins = state.plugins, context = state.context;
              method = action.type;
              isStartEvent = method.match(endsWithStartRegex); // console.log(`data ${method}`, data)
              // console.log(`data allMatches ${method}`, allMatches)

              abortable = data.exact.map(function (x) {
                return x.pluginName;
              });
              /* If abort is called from xyzStart */

              if (isStartEvent) {
                abortable = allMatches.during.map(function (x) {
                  return x.pluginName;
                });
              }
              /* make args for functions to concume */


              makeArgs = argumentFactory(instance, abortable); // console.log('makeArgs', makeArgs)

              /* Check if plugin loaded, if not mark action for queue */

              queueData = data.exact.reduce(function (acc, thing) {
                var pluginName = thing.pluginName,
                    methodName = thing.methodName;
                var addToQueue = false; // Queue actions if plugin not loaded except for initialize and reset

                if (!methodName.match(/^initialize/) && !methodName.match(/^reset/)) {
                  addToQueue = !plugins[pluginName].loaded;
                }
                /* If offline and its a core method. Add to queue */


                if (context.offline && methodName.match(/^(page|track|identify)/)) {
                  addToQueue = true;
                }

                acc["".concat(pluginName)] = addToQueue;
                return acc;
              }, {});
              /* generate plugin specific payloads */

              _context5.next = 10;
              return data.exact.reduce(
              /*#__PURE__*/
              function () {
                var _ref6 = asyncToGenerator(
                /*#__PURE__*/
                regenerator.mark(function _callee3(scoped, curr, i$$1) {
                  var pluginName, curScope, scopedPayload;
                  return regenerator.wrap(function _callee3$(_context3) {
                    while (1) {
                      switch (_context3.prev = _context3.next) {
                        case 0:
                          pluginName = curr.pluginName;
                          _context3.next = 3;
                          return scoped;

                        case 3:
                          curScope = _context3.sent;

                          if (!(data.namespaced && data.namespaced[pluginName])) {
                            _context3.next = 11;
                            break;
                          }

                          _context3.next = 7;
                          return data.namespaced[pluginName].reduce(
                          /*#__PURE__*/
                          function () {
                            var _ref7 = asyncToGenerator(
                            /*#__PURE__*/
                            regenerator.mark(function _callee2(acc, p$$1, count) {
                              var curScopeData, genAbort, val, returnValue;
                              return regenerator.wrap(function _callee2$(_context2) {
                                while (1) {
                                  switch (_context2.prev = _context2.next) {
                                    case 0:
                                      genAbort = function _ref8(currentAct, pname, otherPlug) {
                                        return function (reason, plugins) {
                                          var callsite = otherPlug || pname; // console.log(`__abort msg: ${reason}`)
                                          // console.log(`__abort ${pname}`)
                                          // console.log(`__abort xxx: ${plugins}`)
                                          // console.log(`__abort otherPlug`, otherPlug)

                                          return objectSpread2$1(objectSpread2$1({}, currentAct), {}, {
                                            abort: {
                                              reason: reason,
                                              plugins: plugins || [pname],
                                              caller: method,
                                              from: callsite
                                            }
                                          });
                                        };
                                      };

                                      _context2.next = 3;
                                      return acc;

                                    case 3:
                                      curScopeData = _context2.sent;

                                      if (!(!p$$1.method || !p(p$$1.method))) {
                                        _context2.next = 6;
                                        break;
                                      }

                                      return _context2.abrupt("return", curScopeData);

                                    case 6:
                                      /* Make sure plugins don’t call themselves */
                                      validateMethod(p$$1.methodName, p$$1.pluginName);
                                      _context2.next = 9;
                                      return p$$1.method({
                                        payload: curScopeData,
                                        instance: instance,
                                        abort: genAbort(curScopeData, pluginName, p$$1.pluginName),
                                        config: getConfig(p$$1.pluginName, plugins, allPlugins),
                                        plugins: plugins
                                      });

                                    case 9:
                                      val = _context2.sent;
                                      returnValue = g(val) ? val : {};
                                      return _context2.abrupt("return", Promise.resolve(objectSpread2$1(objectSpread2$1({}, curScopeData), returnValue)));

                                    case 12:
                                    case "end":
                                      return _context2.stop();
                                  }
                                }
                              }, _callee2);
                            }));

                            return function (_x10, _x11, _x12) {
                              return _ref7.apply(this, arguments);
                            };
                          }(), Promise.resolve(action));

                        case 7:
                          scopedPayload = _context3.sent;
                          /* Set scoped payload */

                          curScope[pluginName] = scopedPayload;
                          _context3.next = 12;
                          break;

                        case 11:
                          /* Set payload as default action */
                          curScope[pluginName] = action;

                        case 12:
                          return _context3.abrupt("return", Promise.resolve(curScope));

                        case 13:
                        case "end":
                          return _context3.stop();
                      }
                    }
                  }, _callee3);
                }));

                return function (_x7, _x8, _x9) {
                  return _ref6.apply(this, arguments);
                };
              }(), Promise.resolve({}));

            case 10:
              payloads = _context5.sent;
              _context5.next = 13;
              return data.exact.reduce(
              /*#__PURE__*/
              function () {
                var _ref9 = asyncToGenerator(
                /*#__PURE__*/
                regenerator.mark(function _callee4(promise, curr, i$$1) {
                  var lastLoop, pluginName, currentPlugin, currentActionValue, payloadValue, funcArgs, val, returnValue, merged, scopedPayload, nameSpaceEvent, actionDepth, updatedPayload;
                  return regenerator.wrap(function _callee4$(_context4) {
                    while (1) {
                      switch (_context4.prev = _context4.next) {
                        case 0:
                          lastLoop = data.exact.length === i$$1 + 1;
                          pluginName = curr.pluginName;
                          currentPlugin = allPlugins[pluginName];
                          _context4.next = 5;
                          return promise;

                        case 5:
                          currentActionValue = _context4.sent;
                          payloadValue = payloads[pluginName] ? payloads[pluginName] : {};
                          /* If eventStart, allow for value merging */

                          if (isStartEvent) {
                            payloadValue = currentActionValue;
                          }

                          if (!shouldAbort(payloadValue, pluginName)) {
                            _context4.next = 11;
                            break;
                          } // console.log(`> Abort from payload specific "${pluginName}" abort value`, payloadValue)


                          abortDispatch({
                            data: payloadValue,
                            method: method,
                            instance: instance,
                            pluginName: pluginName,
                            store: store
                          });
                          return _context4.abrupt("return", Promise.resolve(currentActionValue));

                        case 11:
                          if (!shouldAbort(currentActionValue, pluginName)) {
                            _context4.next = 14;
                            break;
                          } // console.log(`> Abort from ${method} abort value`, currentActionValue)


                          if (lastLoop) {
                            abortDispatch({
                              data: currentActionValue,
                              method: method,
                              instance: instance,
                              // pluginName,
                              store: store
                            });
                          }

                          return _context4.abrupt("return", Promise.resolve(currentActionValue));

                        case 14:
                          if (!(queueData.hasOwnProperty(pluginName) && queueData[pluginName] === true)) {
                            _context4.next = 17;
                            break;
                          } // console.log('Queue this instead', pluginName)


                          store.dispatch({
                            type: "queue",
                            plugin: pluginName,
                            payload: payloadValue,

                            /* Internal data for analytics engine */
                            _: {
                              called: "queue",
                              from: 'queueMechanism' // for debugging

                            }
                          });
                          return _context4.abrupt("return", Promise.resolve(currentActionValue));

                        case 17:
                          /*
                          const checkForLoaded = () => {
                            const p = instance.getState('plugins')
                            return p[currentPlugin.name].loaded
                          }
                          // const p = instance.getState('plugins')
                          console.log(`loaded "${currentPlugin.name}" > ${method}:`, p[currentPlugin.name].loaded)
                          // await waitForReady(currentPlugin, checkForLoaded, 10000).then((d) => {
                          //   console.log(`Loaded ${method}`, currentPlugin.name)
                          // }).catch((e) => {
                          //   console.log(`Error ${method} ${currentPlugin.name}`, e)
                          //   // TODO dispatch failure
                          // })
                          */
                          // @TODO figure out if we want queuing semantics
                          funcArgs = makeArgs(payloads[pluginName], allPlugins[pluginName]); // console.log(`funcArgs ${method} ${pluginName}`, funcArgs)

                          /* Run the plugin function */

                          _context4.next = 20;
                          return currentPlugin[method]({
                            // currentPlugin: pluginName,
                            abort: funcArgs.abort,
                            // Send in original action value or scope payload
                            payload: payloadValue,
                            instance: instance,
                            config: getConfig(pluginName, plugins, allPlugins),
                            plugins: plugins
                          });

                        case 20:
                          val = _context4.sent;
                          returnValue = g(val) ? val : {};
                          merged = objectSpread2$1(objectSpread2$1({}, currentActionValue), returnValue);
                          scopedPayload = payloads[pluginName]; // || currentActionValue

                          if (shouldAbort(scopedPayload, pluginName)) {
                            // console.log(`>> HANDLE abort ${method} ${pluginName}`)
                            abortDispatch({
                              data: scopedPayload,
                              method: method,
                              instance: instance,
                              pluginName: pluginName,
                              store: store
                            });
                          } else {
                            nameSpaceEvent = "".concat(method, ":").concat(pluginName);
                            actionDepth = (nameSpaceEvent.match(/:/g) || []).length;

                            if (actionDepth < 2 && !method.match(bootstrapRegex) && !method.match(readyRegex)) {
                              updatedPayload = isStartEvent ? merged : payloadValue; // Dispatched for `.on('xyz') listeners.

                              instance.dispatch(objectSpread2$1(objectSpread2$1({}, updatedPayload), {}, {
                                type: nameSpaceEvent,
                                _: {
                                  called: nameSpaceEvent,
                                  from: 'submethod'
                                }
                              }));
                            }
                          } // console.log('merged', merged)


                          return _context4.abrupt("return", Promise.resolve(merged));

                        case 26:
                        case "end":
                          return _context4.stop();
                      }
                    }
                  }, _callee4);
                }));

                return function (_x13, _x14, _x15) {
                  return _ref9.apply(this, arguments);
                };
              }(), Promise.resolve(action));

            case 13:
              resolvedAction = _context5.sent;

              if (!(!method.match(endsWithStartRegex) && !method.match(/^registerPlugin/) && // !method.match(/^disablePlugin/) &&
              // !method.match(/^enablePlugin/) &&
              !method.match(readyRegex) && !method.match(bootstrapRegex) && !method.match(/^params/) && !method.match(/^userIdChanged/))) {
                _context5.next = 21;
                break;
              }

              if (EVENTS$$1.plugins.includes(method)) ; // console.log(`Dont dispatch for ${method}`, resolvedAction)
              // return resolvedAction

              /*
                Verify this original action setup.
                It's intended to keep actions from double dispatching themselves
              */

              if (!(resolvedAction._ && resolvedAction._.originalAction === method)) {
                _context5.next = 18;
                break;
              }

              return _context5.abrupt("return", resolvedAction);

            case 18:
              endAction = objectSpread2$1(objectSpread2$1({}, resolvedAction), {
                _: {
                  originalAction: resolvedAction.type,
                  called: resolvedAction.type,
                  from: 'engineEnd'
                }
              });
              /* If all plugins are aborted, dispatch xAborted */

              if (shouldAbortAll(resolvedAction, data.exact.length) && !method.match(/End$/)) {
                endAction = objectSpread2$1(objectSpread2$1({}, endAction), {
                  type: resolvedAction.type + 'Aborted'
                });
              }

              store.dispatch(endAction);

            case 21:
              return _context5.abrupt("return", resolvedAction);

            case 22:
            case "end":
              return _context5.stop();
          }
        }
      }, _callee5);
    }));
    return _processEvent.apply(this, arguments);
  }

  function abortDispatch(_ref3) {
    var data = _ref3.data,
        method = _ref3.method,
        instance = _ref3.instance,
        pluginName = _ref3.pluginName,
        store = _ref3.store;
    var postFix = pluginName ? ':' + pluginName : '';
    var abortEvent = method + 'Aborted' + postFix;
    store.dispatch(objectSpread2$1(objectSpread2$1({}, data), {}, {
      type: abortEvent,
      _: {
        called: abortEvent,
        from: 'abort'
      }
    }));
  }

  function getConfig(name, pluginState, allPlugins) {
    var pluginData = pluginState[name] || allPlugins[name];

    if (pluginData && pluginData.config) {
      return pluginData.config;
    }

    return {};
  }

  function getPluginFunctions(methodName, plugins) {
    return plugins.reduce(function (arr, plugin) {
      return !plugin[methodName] ? arr : arr.concat({
        methodName: methodName,
        pluginName: plugin.name,
        method: plugin[methodName]
      });
    }, []);
  }

  function formatMethod(type) {
    return type.replace(endsWithStartRegex, '');
  }
  /**
   * Return array of event names
   * @param  {String} eventType - original event type
   * @param  {String} namespace - optional namespace postfix
   * @return {array} - type, method, end
   */


  function getEventNames(eventType, namespace) {
    var method = formatMethod(eventType);
    var postFix = namespace ? ":".concat(namespace) : ''; // `typeStart:pluginName`

    var type = "".concat(eventType).concat(postFix); // `type:pluginName`

    var methodName = "".concat(method).concat(postFix); // `typeEnd:pluginName`

    var end = "".concat(method, "End").concat(postFix);
    return [type, methodName, end];
  }
  /* Collect all calls for a given event in the system */


  function getAllMatchingCalls(eventType, activePlugins, allPlugins) {
    var eventNames = getEventNames(eventType); // console.log('eventNames', eventNames)
    // 'eventStart', 'event', & `eventEnd`

    var core = eventNames.map(function (word) {
      return getPluginFunctions(word, activePlugins);
    }); // Gather nameSpaced Events

    return activePlugins.reduce(function (acc, plugin) {
      var name = plugin.name;
      var nameSpacedEvents = getEventNames(eventType, name); // console.log('eventNames namespaced', nameSpacedEvents)

      var _nameSpacedEvents$map = nameSpacedEvents.map(function (word) {
        return getPluginFunctions(word, activePlugins);
      }),
          _nameSpacedEvents$map2 = slicedToArray(_nameSpacedEvents$map, 3),
          beforeFuncs = _nameSpacedEvents$map2[0],
          duringFuncs = _nameSpacedEvents$map2[1],
          afterFuncs = _nameSpacedEvents$map2[2];

      if (beforeFuncs.length) {
        acc.beforeNS[name] = beforeFuncs;
      }

      if (duringFuncs.length) {
        acc.duringNS[name] = duringFuncs;
      }

      if (afterFuncs.length) {
        acc.afterNS[name] = afterFuncs;
      }

      return acc;
    }, {
      before: core[0],
      beforeNS: {},
      during: core[1],
      duringNS: {},
      after: core[2],
      afterNS: {}
    });
  }

  function shouldAbort(_ref4, pluginName) {
    var abort = _ref4.abort;
    if (!abort) return false;
    if (abort === true) return true;
    return includes(abort, pluginName) || abort && includes(abort.plugins, pluginName);
  }

  function shouldAbortAll(_ref5, pluginsCount) {
    var abort = _ref5.abort;
    if (!abort) return false;
    if (abort === true || a(abort)) return true;
    var plugins = abort.plugins;
    return isArray$1(abort) && abort.length === pluginsCount || isArray$1(plugins) && plugins.length === pluginsCount;
  }

  function isArray$1(arr) {
    return Array.isArray(arr);
  }

  function includes(arr, name) {
    if (!arr || !isArray$1(arr)) return false;
    return arr.includes(name);
  }
  /**
   * Generate arguments to pass to plugin methods
   * @param  {Object} instance - analytics instance
   * @param  {array} abortablePlugins - plugins that can be cancelled by caller
   * @return {*} function to inject plugin params
   */


  function argumentFactory(instance, abortablePlugins) {
    // console.log('abortablePlugins', abortablePlugins)
    return function (action, plugin, otherPlugin) {
      var config = plugin.config,
          name = plugin.name;
      var method = "".concat(name, ".").concat(action.type);

      if (otherPlugin) {
        method = otherPlugin.event;
      }

      var abortF = action.type.match(endsWithStartRegex) ? abortFunction(name, method, abortablePlugins, otherPlugin, action) : notAbortableError(action, method);
      return {
        /* self: plugin, for future maybe */
        // clone objects to avoid reassign
        payload: formatPayload(action),
        instance: instance,
        config: config || {},
        abort: abortF
      };
    };
  }

  function abortFunction(pluginName, method, abortablePlugins, otherPlugin, action) {
    return function (reason, plugins) {
      var caller = otherPlugin ? otherPlugin.name : pluginName;
      var pluginsToAbort = plugins && isArray$1(plugins) ? plugins : abortablePlugins;

      if (otherPlugin) {
        pluginsToAbort = plugins && isArray$1(plugins) ? plugins : [pluginName];

        if (!pluginsToAbort.includes(pluginName) || pluginsToAbort.length !== 1) {
          throw new Error("Method ".concat(method, " can only abort ").concat(pluginName, " plugin. ").concat(JSON.stringify(pluginsToAbort), " input valid"));
        }
      }

      return objectSpread2$1(objectSpread2$1({}, action), {}, {
        // 🔥 todo verify this merge is ok
        abort: {
          reason: reason,
          plugins: pluginsToAbort,
          caller: method,
          _: caller
        }
      });
    };
  }

  function notAbortableError(action, method) {
    return function () {
      throw new Error(action.type + ' action not cancellable. Remove abort in ' + method);
    };
  }
  /**
   * Verify plugin is not calling itself with whatever:myPluginName self refs
   */


  function validateMethod(actionName, pluginName) {
    var text = getNameSpacedAction(actionName);
    var methodCallMatchesPluginNamespace = text && text.name === pluginName;

    if (methodCallMatchesPluginNamespace) {
      var sub = getNameSpacedAction(text.method);
      var subText = sub ? 'or ' + sub.method : '';
      throw new Error([pluginName + ' plugin is calling method ' + actionName, 'Plugins cant call self', "Use ".concat(text.method, " ").concat(subText, " in ").concat(pluginName, " plugin insteadof ").concat(actionName)].join('\n'));
    }
  }

  function getNameSpacedAction(event) {
    var split = event.match(/(.*):(.*)/);

    if (!split) {
      return false;
    }

    return {
      method: split[1],
      name: split[2]
    };
  }

  function formatPayload(action) {
    return Object.keys(action).reduce(function (acc, key) {
      // redact type from { payload }
      if (key === 'type') {
        return acc;
      }

      if (g(action[key])) {
        acc[key] = Object.assign({}, action[key]);
      } else {
        acc[key] = action[key];
      }

      return acc;
    }, {});
  }
  /*
  function getMatchingMethods(eventType, activePlugins) {
    const exact = getPluginFunctions(eventType, activePlugins)
    // console.log('exact', exact)
    // Gather nameSpaced Events
    return activePlugins.reduce((acc, plugin) => {
      const { name } = plugin
      const clean = getPluginFunctions(`${eventType}:${name}`, activePlugins)
      if (clean.length) {
        acc.namespaced[name] = clean
      }
      return acc
    }, {
      exact: exact,
      namespaced: {}
    })
  }
  */


  function pluginMiddleware(instance, getPlugins, systemEvents) {
    var isReady = {};
    return function (store) {
      return function (next) {
        return (
          /*#__PURE__*/
          function () {
            var _ref = asyncToGenerator(
            /*#__PURE__*/
            regenerator.mark(function _callee(action) {
              var type, abort, plugins, updatedAction, allPlugins, pluginsArray, allRegisteredPlugins, completed, failed, disabled, waitForPluginsToLoad, updated;
              return regenerator.wrap(function _callee$(_context) {
                while (1) {
                  switch (_context.prev = _context.next) {
                    case 0:
                      type = action.type, abort = action.abort, plugins = action.plugins;
                      updatedAction = action;

                      if (!abort) {
                        _context.next = 4;
                        break;
                      }

                      return _context.abrupt("return", next(action));

                    case 4:
                      /* Analytics.plugins.enable called, we need to init the plugins */
                      if (type === EVENTS.enablePlugin) {
                        store.dispatch({
                          type: EVENTS.initializeStart,
                          plugins: plugins,
                          disabled: [],
                          fromEnable: true,
                          meta: action.meta
                        });
                      }

                      if (type === EVENTS.disablePlugin) {
                        // If cached callback, resolve promise/run callback. debounced to fix race condition
                        setTimeout(function () {
                          return runCallback(action.meta.rid, {
                            payload: action
                          });
                        }, 0);
                      }
                      /* @TODO implement
                      if (type === EVENTS.loadPlugin) {
                        // Rerun initialize calls in plugins
                        const allPlugins = getPlugins()
                        const pluginsToLoad = Object.keys(allPlugins).filter((name) => {
                          return plugins.includes(name)
                        }).reduce((acc, curr) => {
                          acc[curr] = allPlugins[curr]
                          return acc
                        }, {})
                        const initializeAction = {
                          type: EVENTS.initializeStart,
                          plugins: plugins
                        }
                        const updated = await runPlugins(initializeAction, pluginsToLoad, instance, store, systemEvents)
                        return next(updated)
                      }
                      */
                      //  || type.match(/^initializeAbort:/)


                      if (type === EVENTS.initializeEnd) {
                        allPlugins = getPlugins();
                        pluginsArray = Object.keys(allPlugins);
                        allRegisteredPlugins = pluginsArray.filter(function (name) {
                          return plugins.includes(name);
                        }).map(function (name) {
                          return allPlugins[name];
                        });
                        completed = [];
                        failed = [];
                        disabled = action.disabled; // console.log('allRegisteredPlugins', allRegisteredPlugins)

                        waitForPluginsToLoad = allRegisteredPlugins.map(function (plugin) {
                          var loaded = plugin.loaded,
                              name = plugin.name;
                          /* Plugins will abort trying to load after 10 seconds. 1e4 === 10000 MS */

                          return waitForReady(plugin, loaded, 1e4).then(function (d$$1) {
                            if (!isReady[name]) {
                              // only dispatch namespaced rdy once
                              store.dispatch({
                                type: EVENTS.pluginReadyType(name),
                                // `ready:${name}`
                                name: name,
                                events: Object.keys(plugin).filter(function (name) {
                                  return !nonEvents.includes(name);
                                })
                              });
                              isReady[name] = true;
                            }

                            completed = completed.concat(name);
                            return plugin; // It's loaded! run the command
                          })["catch"](function (e$$1) {
                            // Timeout Add to queue
                            // console.log('Error generic waitForReady. Push this to queue', e)
                            if (e$$1 instanceof Error) {
                              throw new Error(e$$1);
                            }

                            failed = failed.concat(e$$1.name); // Failed to fire, add to queue

                            return e$$1;
                          });
                        });
                        Promise.all(waitForPluginsToLoad).then(function (calls) {
                          // setTimeout to ensure runs after 'page'
                          var payload = {
                            plugins: completed,
                            failed: failed,
                            disabled: disabled
                          };
                          setTimeout(function () {
                            if (pluginsArray.length === waitForPluginsToLoad.length + disabled.length) {
                              store.dispatch(objectSpread2$1(objectSpread2$1({}, {
                                type: EVENTS.ready
                              }), payload));
                            }
                          }, 0);
                        });
                      }
                      /* New plugin system */


                      if (!(type !== EVENTS.bootstrap)) {
                        _context.next = 13;
                        break;
                      }

                      if (/^ready:([^:]*)$/.test(type)) {
                        // Immediately flush queue
                        setTimeout(function () {
                          return processQueue(store, getPlugins, instance);
                        }, 0);
                      }

                      _context.next = 11;
                      return runPlugins(action, getPlugins, instance, store, systemEvents);

                    case 11:
                      updated = _context.sent;
                      return _context.abrupt("return", next(updated));

                    case 13:
                      return _context.abrupt("return", next(updatedAction));

                    case 14:
                    case "end":
                      return _context.stop();
                  }
                }
              }, _callee);
            }));

            return function (_x) {
              return _ref.apply(this, arguments);
            };
          }()
        );
      };
    };
  }

  function storageMiddleware(storage) {
    return function (store) {
      return function (next) {
        return function (action) {
          var type = action.type,
              key = action.key,
              value = action.value,
              options = action.options;

          if (type === EVENTS.setItem || type === EVENTS.removeItem) {
            if (action.abort) {
              return next(action);
            } // Run storage set or remove


            if (type === EVENTS.setItem) {
              storage.setItem(key, value, options);
            } else {
              storage.removeItem(key, options);
            }
          }

          return next(action);
        };
      };
    };
  }
  /*
    Todo: emit events for keys we care about
    window.addEventListener('storage', (event) => console.log(event));
  */


  function _classCallCheck(instance, Constructor) {
    if (!(instance instanceof Constructor)) {
      throw new TypeError("Cannot call a class as a function");
    }
  }

  var classCallCheck = _classCallCheck;
  /* Class to fix dynamic middlewares from conflicting with each other
  if more than one analytic instance is active */

  var DynamicMiddleware = function DynamicMiddleware() {
    var _this = this;

    classCallCheck(this, DynamicMiddleware);
    defineProperty$1(this, "before", []);
    defineProperty$1(this, "after", []);
    defineProperty$1(this, "addMiddleware", function (middlewares, position) {
      _this[position] = _this[position].concat(middlewares);
    });
    defineProperty$1(this, "removeMiddleware", function (middleware, position) {
      var index = _this[position].findIndex(function (d$$1) {
        return d$$1 === middleware;
      });

      if (index === -1) return;
      _this[position] = [].concat(toConsumableArray(_this[position].slice(0, index)), toConsumableArray(_this[position].slice(index + 1)));
    });
    defineProperty$1(this, "dynamicMiddlewares", function (position) {
      return function (store) {
        return function (next) {
          return function (action) {
            var middlewareAPI = {
              getState: store.getState,
              dispatch: function dispatch(act) {
                return store.dispatch(act);
              }
            };

            var chain = _this[position].map(function (middleware) {
              return middleware(middlewareAPI);
            });

            return compose.apply(void 0, toConsumableArray(chain))(next)(action);
          };
        };
      };
    });
  };

  function createReducer(getPlugins) {
    return function plugins() {
      var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
      var action = arguments.length > 1 ? arguments[1] : undefined;
      var newState = {};

      if (action.type === 'initialize:aborted') {
        return state;
      }

      if (/^registerPlugin:([^:]*)$/.test(action.type)) {
        var name = getNameFromEventType(action.type, 'registerPlugin');
        var plugin = getPlugins()[name];

        if (!plugin || !name) {
          return state;
        }

        var isEnabled = action.enabled;
        newState[name] = {
          enabled: isEnabled,

          /* if no initialization method. Set initialized true */
          initialized: isEnabled ? Boolean(!plugin.initialize) : false,

          /* If plugin enabled === false, set loaded to false, else check plugin.loaded function */
          loaded: isEnabled ? Boolean(plugin.loaded()) : false,
          config: plugin.config || {}
        };
        return objectSpread2$1(objectSpread2$1({}, state), newState);
      }

      if (/^initialize:([^:]*)$/.test(action.type)) {
        var _name = getNameFromEventType(action.type, EVENTS.initialize);

        var _plugin = getPlugins()[_name];

        if (!_plugin || !_name) {
          return state;
        }

        newState[_name] = objectSpread2$1(objectSpread2$1({}, state[_name]), {
          initialized: true,

          /* check plugin.loaded function */
          loaded: Boolean(_plugin.loaded())
        });
        return objectSpread2$1(objectSpread2$1({}, state), newState);
      }

      if (/^ready:([^:]*)$/.test(action.type)) {
        // const name = getNameFromEventType(action.type, 'ready')
        newState[action.name] = objectSpread2$1(objectSpread2$1({}, state[action.name]), {
          loaded: true
        });
        return objectSpread2$1(objectSpread2$1({}, state), newState);
      }

      switch (action.type) {
        /* case EVENTS.pluginFailed:
          // console.log('PLUGIN_FAILED', action.name)
          newState[action.name] = {
            ...state[action.name],
            ...{ loaded: false }
          }
          return { ...state, ...newState }
        */

        /* When analytics.plugins.disable called */
        case EVENTS.disablePlugin:
          return objectSpread2$1(objectSpread2$1({}, state), togglePluginStatus(action.plugins, false, state));

        /* When analytics.plugins.enable called */

        case EVENTS.enablePlugin:
          return objectSpread2$1(objectSpread2$1({}, state), togglePluginStatus(action.plugins, true, state));

        default:
          return state;
      }
    };
  }

  function getNameFromEventType(type, baseName) {
    return type.substring(baseName.length + 1, type.length);
  }

  function togglePluginStatus(plugins, status, currentState) {
    return plugins.reduce(function (acc, pluginKey) {
      acc[pluginKey] = objectSpread2$1(objectSpread2$1({}, currentState[pluginKey]), {
        enabled: status
      });
      return acc;
    }, currentState);
  }

  function serialize(obj) {
    try {
      return JSON.parse(JSON.stringify(obj));
    } catch (err) {}

    return obj;
  }

  var initialState = {
    last: {},
    history: [] // track reducer

  };

  function trackReducer() {
    var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : initialState;
    var action = arguments.length > 1 ? arguments[1] : undefined;
    var type = action.type,
        event = action.event,
        properties = action.properties,
        options = action.options,
        meta = action.meta;

    switch (type) {
      case EVENTS.track:
        var trackEvent = serialize(objectSpread2$1(objectSpread2$1({
          event: event,
          properties: properties
        }, Object.keys(options).length && {
          options: options
        }), {}, {
          meta: meta
        }));
        return objectSpread2$1(objectSpread2$1({}, state), {
          last: trackEvent,
          // Todo prevent LARGE arrays https://bit.ly/2MnBwPT
          history: state.history.concat(trackEvent)
        });

      default:
        return state;
    }
  }
  /*
  TODO figure out if this should live in state...
  Queue could be in mermory as well.
  But also needs to be persisted to storage
  */


  var initialState$1 = {
    actions: []
  };

  function queueReducer() {
    var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : initialState$1;
    var action = arguments.length > 1 ? arguments[1] : undefined;
    var type = action.type,
        payload = action.payload;

    switch (type) {
      case 'queue':
        var actionChain;
        /* prioritize identify in event queue */

        if (payload && payload.type && payload.type === EVENTS.identify) {
          actionChain = [action].concat(state.actions);
        } else {
          actionChain = state.actions.concat(action);
        }

        return objectSpread2$1(objectSpread2$1({}, state), {}, {
          actions: actionChain
        });

      case 'dequeue':
        return [];
      // todo push events to history

      default:
        return state;
    }
  }

  var hashRegex = /#.*$/;

  function canonicalUrl() {
    if (!c) return;
    var tags = document.getElementsByTagName('link');

    for (var i$$1 = 0, tag; tag = tags[i$$1]; i$$1++) {
      if (tag.getAttribute('rel') === 'canonical') {
        return tag.getAttribute('href');
      }
    }
  }

  function urlPath(url) {
    var regex = /(http[s]?:\/\/)?([^\/\s]+\/)(.*)/g;
    var matches = regex.exec(url);
    var pathMatch = matches && matches[3] ? matches[3].split('?')[0].replace(hashRegex, '') : '';
    return '/' + pathMatch;
  }
  /**
   * Return the canonical URL and rmove the hash.
   * @param  {string} search - search param
   * @return {string} return current canonical URL
   */


  function currentUrl(search) {
    var canonical = canonicalUrl();
    if (!canonical) return window.location.href.replace(hashRegex, '');
    return canonical.match(/\?/) ? canonical : canonical + search;
  }
  /**
   * Page data for overides
   * @typedef {object} PageData
   * @property {string} [title] - Page title
   * @property {string} [url] - Page url
   * @property {string} [path] - Page path
   * @property {string} [search] - Page search
   * @property {string} [width] - Page width
   * @property {string} [height] - Page height
  */

  /**
   * Get information about current page
   * @typedef {Function} getPageData
   * @param  {PageData} [pageData = {}] - Page data overides
   * @return {PageData} resolved page data
   */


  var getPageData = function getPageData() {
    var pageData = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
    if (!c) return pageData;
    var _document = document,
        title = _document.title,
        referrer = _document.referrer;
    var _window = window,
        location = _window.location,
        innerWidth = _window.innerWidth,
        innerHeight = _window.innerHeight;
    var hash = location.hash,
        search = location.search;
    var url = currentUrl(search);
    var page = {
      title: title,
      url: url,
      path: urlPath(url),
      hash: hash,
      search: search,
      width: innerWidth,
      height: innerHeight
    };

    if (referrer && referrer !== '') {
      page.referrer = referrer;
    }

    return objectSpread2$1(objectSpread2$1({}, page), pageData);
  };

  var initialState$2 = {
    last: {},
    history: [] // page reducer

  };

  function page() {
    var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : initialState$2;
    var action = arguments.length > 1 ? arguments[1] : undefined;
    var properties = action.properties,
        options = action.options,
        meta = action.meta;

    switch (action.type) {
      case EVENTS.page:
        var viewData = serialize(objectSpread2$1({
          properties: properties,
          meta: meta
        }, Object.keys(options).length && {
          options: options
        }));
        return objectSpread2$1(objectSpread2$1({}, state), {
          last: viewData,
          // Todo prevent LARGE arrays https://bit.ly/2MnBwPT
          history: state.history.concat(viewData)
        });

      default:
        return state;
    }
  } // TODO fix os. os getting stripped out for node build


  function getNodeOS() {
    return 'na'; // os.platform()
  }

  var osName;
  var referrer;
  var locale;
  var timeZone;
  {
    osName = getNodeOS();
    referrer = {};
  }
  var initialState$3 = {
    initialized: false,
    sessionId: y$1(),
    app: null,
    version: null,
    debug: false,
    offline: c ? !navigator.onLine : false,
    // use node network is-online
    os: {
      name: osName
    },
    userAgent: c ? navigator.userAgent : 'node',
    // https://github.com/bestiejs/platform.js
    library: {
      name: LIBRARY_NAME,
      // TODO fix version number. npm run publish:patch has wrong version
      version: "0.10.13"
    },
    timezone: timeZone,
    locale: locale,
    campaign: {},
    referrer: referrer // context reducer

  };

  function context() {
    var state = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : initialState$3;
    var action = arguments.length > 1 ? arguments[1] : undefined;
    var initialized = state.initialized;
    var type = action.type,
        campaign = action.campaign;

    switch (type) {
      case EVENTS.campaign:
        return objectSpread2$1(objectSpread2$1({}, state), {
          campaign: campaign
        });

      case EVENTS.offline:
        return objectSpread2$1(objectSpread2$1({}, state), {
          offline: true
        });

      case EVENTS.online:
        return objectSpread2$1(objectSpread2$1({}, state), {
          offline: false
        });

      default:
        if (!initialized) {
          return objectSpread2$1(objectSpread2$1(objectSpread2$1({}, initialState$3), state), {
            initialized: true
          });
        }

        return state;
    }
  }

  var excludeItems = ['plugins', 'reducers', 'storage']; // Pull plugins and reducers off intital config

  function makeContext(config) {
    return Object.keys(config).reduce(function (acc, current) {
      if (excludeItems.includes(current)) {
        return acc;
      }

      acc[current] = config[current];
      return acc;
    }, {});
  }

  function Debug() {
    // initialize global history
    var globalVariable = PREFIX + LIBRARY_NAME + PREFIX; // Global key is window.__analytics__

    f$2(globalVariable, []); // Return debugger

    return function (createStore$$1) {
      return function (reducer, preloadedState, enhancer) {
        var store = createStore$$1(reducer, preloadedState, enhancer);
        var origDispatch = store.dispatch;

        var dispatch = function dispatch(action) {
          var a$$1 = action.action || action;
          l$2[globalVariable].push(a$$1);
          return origDispatch(action);
        };

        return Object.assign(store, {
          dispatch: dispatch
        });
      };
    };
  }

  function composeWithDebug(config) {
    return function () {
      return compose(compose.apply(null, arguments), Debug(config));
    };
  }

  function ensureArray(singleOrArray) {
    if (!singleOrArray) return [];
    if (b(singleOrArray)) return singleOrArray;
    return [singleOrArray];
  }
  /**
   * Grab first function found from arguments
   * @param {array} [argArray] - arguments array to find first function
   * @returns {Function|undefined}
   */


  function getCallbackFromArgs(argArray) {
    var args = argArray || Array.prototype.slice.call(arguments);
    var cb;

    for (var i$$1 = 0; i$$1 < args.length; i$$1++) {
      if (p(args[i$$1])) {
        cb = args[i$$1];
        break;
      }
    }

    return cb;
  }

  function timeStamp() {
    return new Date().getTime();
  }

  function deferredPromiseResolver(resolver, callback) {
    return function (data) {
      if (callback) callback(data);
      resolver(data);
    };
  }

  function generateMeta() {
    var meta = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
    var resolve = arguments.length > 1 ? arguments[1] : undefined;
    var possibleCallbacks = arguments.length > 2 ? arguments[2] : undefined;
    var rid = y$1();

    if (resolve) {
      // stack[`${rid}-info`] = meta
      stack[rid] = deferredPromiseResolver(resolve, getCallbackFromArgs(possibleCallbacks));
    }

    return objectSpread2$1(objectSpread2$1({}, meta), {}, {
      rid: rid,
      ts: timeStamp()
    }, !resolve ? {} : {
      hasCallback: true
    });
  }
  /**
   * Analytics library configuration
   *
   * After the library is initialized with config, the core API is exposed & ready for use in the application.
   *
   * @param {object} config - analytics core config
   * @param {string} [config.app] - Name of site / app
   * @param {string} [config.version] - Version of your app
   * @param {boolean} [config.debug] - Should analytics run in debug mode
   * @param {Array.<AnalyticsPlugin>}  [config.plugins] - Array of analytics plugins
   * @return {AnalyticsInstance} Analytics Instance
   * @example
   *
   * import Analytics from 'analytics'
   * import pluginABC from 'analytics-plugin-abc'
   * import pluginXYZ from 'analytics-plugin-xyz'
   *
   * // initialize analytics
   * const analytics = Analytics({
   *   app: 'my-awesome-app',
   *   plugins: [
   *     pluginABC,
   *     pluginXYZ
   *   ]
   * })
   *
   */


  function analytics() {
    var config = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
    var customReducers = config.reducers || {};
    var initialUser = config.initialUser || {}; // @TODO add custom value reolvers for userId and anonId
    // const resolvers = config.resolvers || {}

    /* Parse plugins array */

    var parsedOptions = (config.plugins || []).reduce(function (acc, plugin) {
      if (p(plugin)) {
        /* Custom redux middleware */
        acc.middlewares = acc.middlewares.concat(plugin);
        return acc;
      } // Legacy plugin with name


      if (plugin.NAMESPACE) plugin.name = plugin.NAMESPACE;

      if (!plugin.name) {
        /* Plugins must supply a "name" property. See error url for more details */
        throw new Error(ERROR_URL + '1');
      } // if plugin exposes EVENTS capture available events


      var definedEvents = plugin.EVENTS ? Object.keys(plugin.EVENTS).map(function (k) {
        return plugin.EVENTS[k];
      }) : [];
      var enabledFromMerge = !(plugin.enabled === false);
      var enabledFromPluginConfig = !(plugin.config && plugin.config.enabled === false); // top level { enabled: false } takes presidence over { config: enabled: false }

      acc.pluginEnabled[plugin.name] = enabledFromMerge && enabledFromPluginConfig;
      delete plugin.enabled;

      if (plugin.methods) {
        acc.methods[plugin.name] = Object.keys(plugin.methods).reduce(function (a$$1, c$$1) {
          // enrich methods with analytics instance
          a$$1[c$$1] = appendArguments(plugin.methods[c$$1]);
          return a$$1;
        }, {}); // Remove additional methods from plugins

        delete plugin.methods;
      } // Convert available methods into events


      var methodsToEvents = Object.keys(plugin); // Combine events

      var allEvents = methodsToEvents.concat(definedEvents); // Dedupe events list

      var allEventsUnique = new Set(acc.events.concat(allEvents));
      acc.events = Array.from(allEventsUnique);
      acc.pluginsArray = acc.pluginsArray.concat(plugin);

      if (acc.plugins[plugin.name]) {
        throw new Error(plugin.name + 'AlreadyLoaded');
      }

      acc.plugins[plugin.name] = plugin;

      if (!acc.plugins[plugin.name].loaded) {
        // set default loaded func
        acc.plugins[plugin.name].loaded = function () {
          return true;
        };
      }

      return acc;
    }, {
      plugins: {},
      pluginEnabled: {},
      methods: {},
      pluginsArray: [],
      middlewares: [],
      events: []
    });
    /* Storage by default is set to global & is not persisted */

    var storage = config.storage ? config.storage : {
      getItem: o$2,
      setItem: f$2,
      removeItem: n$2
    };
    var getUserProp = getUserPropFunc(storage); // mutable intregrations object for dynamic loading

    var customPlugins = parsedOptions.plugins;
    /* Grab all registered events from plugins loaded */

    var allPluginEvents = parsedOptions.events.filter(function (name) {
      return !nonEvents.includes(name);
    }).sort();
    var uniqueEvents = new Set(allPluginEvents.concat(coreEvents).filter(function (name) {
      return !nonEvents.includes(name);
    }));
    var allSystemEvents = Array.from(uniqueEvents).sort();
    /* plugin methods(functions) must be kept out of state. thus they live here */

    var getPlugins = function getPlugins() {
      return customPlugins;
    };

    var _ref = new DynamicMiddleware(),
        addMiddleware = _ref.addMiddleware,
        removeMiddleware = _ref.removeMiddleware,
        dynamicMiddlewares = _ref.dynamicMiddlewares;

    var nonAbortable = function nonAbortable() {
      // throw new Error(`${ERROR_URL}3`)
      throw new Error('Abort disabled inListener');
    }; // Parse URL parameters


    var params = s$1(); // Initialize visitor information

    var persistedUser = getPersistedUserData(storage);
    var visitorInfo = objectSpread2$1(objectSpread2$1(objectSpread2$1(objectSpread2$1({}, persistedUser), initialUser), !params.an_uid ? {} : {
      userId: params.an_uid
    }), !params.an_aid ? {} : {
      anonymousId: params.an_aid
    }); // If no anon id set, create one

    if (!visitorInfo.anonymousId) {
      visitorInfo.anonymousId = y$1();
    }
    /**
     * Async Management methods for plugins. 
     * 
     * This is also where [custom methods](https://bit.ly/329vFXy) are loaded into the instance.
     * @typedef {Object} Plugins
     * @property {EnablePlugin} enable - Set storage value
     * @property {DisablePlugin} disable - Remove storage value
     * @example
     *
     * // Enable a plugin by namespace
     * analytics.plugins.enable('keenio')
     *
     * // Disable a plugin by namespace
     * analytics.plugins.disable('google-analytics')
     */


    var plugins = objectSpread2$1({
      /**
       * Enable analytics plugin
       * @typedef {Function} EnablePlugin
       * @param  {String|Array} plugins - name of plugins(s) to disable
       * @param  {Function} [callback] - callback after enable runs
       * @returns {Promise}
       * @example
       *
       * analytics.plugins.enable('google-analytics').then(() => {
       *   console.log('do stuff')
       * })
       *
       * // Enable multiple plugins at once
       * analytics.plugins.enable(['google-analytics', 'segment']).then(() => {
       *   console.log('do stuff')
       * })
       */
      enable: function enable(plugins, callback) {
        return new Promise(function (resolve) {
          store.dispatch({
            type: EVENTS.enablePlugin,
            plugins: ensureArray(plugins),
            _: {
              originalAction: EVENTS.enablePlugin
            }
          }, resolve, [callback]);
        });
      },

      /**
       * Disable analytics plugin
       * @typedef {Function} DisablePlugin
       * @param  {String|Array} plugins - name of integration(s) to disable
       * @param  {Function} callback - callback after disable runs
       * @returns {Promise}
       * @example
       *
       * analytics.plugins.disable('google').then(() => {
       *   console.log('do stuff')
       * })
       *
       * analytics.plugins.disable(['google', 'segment']).then(() => {
       *   console.log('do stuff')
       * })
       */
      disable: function disable(plugins, callback) {
        return new Promise(function (resolve) {
          store.dispatch({
            type: EVENTS.disablePlugin,
            plugins: ensureArray(plugins),
            _: {
              originalAction: EVENTS.disablePlugin
            }
          }, resolve, [callback]);
        });
      }
    }, parsedOptions.methods);
    /**
     * Analytic instance returned from initialization
     * @typedef {Object} AnalyticsInstance
     * @property {Identify} identify - Identify a user
     * @property {Track} track - Track an analytics event
     * @property {Page} page - Trigger page view
     * @property {User} user - Get user data
     * @property {Reset} reset - Clear information about user & reset analytics
     * @property {Ready} ready - Fire callback on analytics ready event
     * @property {On} on - Fire callback on analytics lifecycle events.
     * @property {Once} once - Fire callback on analytics lifecycle events once.
     * @property {GetState} getState - Get data about user, activity, or context.
     * @property {Storage} storage - storage methods
     * @property {Plugins} plugins - plugin methods
     */

    var instance = {
      /**
      * Identify a user. This will trigger `identify` calls in any installed plugins and will set user data in localStorage
      * @typedef {Function} Identify
      * @param  {String}   userId  - Unique ID of user
      * @param  {Object}   [traits]  - Object of user traits
      * @param  {Object}   [options] - Options to pass to identify call
      * @param  {Function} [callback] - Callback function after identify completes
      * @returns {Promise}
      * @api public
      *
      * @example
      *
      * // Basic user id identify
      * analytics.identify('xyz-123')
      *
      * // Identify with additional traits
      * analytics.identify('xyz-123', {
      *   name: 'steve',
      *   company: 'hello-clicky'
      * })
      *
      * // Fire callback with 2nd or 3rd argument
      * analytics.identify('xyz-123', () => {
      *   console.log('do this after identify')
      * })
      *
      * // Disable sending user data to specific analytic tools
      * analytics.identify('xyz-123', {}, {
      *   plugins: {
      *     // disable sending this identify call to segment
      *     segment: false
      *   }
      * })
      *
      * // Send user data to only to specific analytic tools
      * analytics.identify('xyz-123', {}, {
      *   plugins: {
      *     // disable this specific identify in all plugins except customerio
      *     all: false,
      *     customerio: true
      *   }
      * })
      */
      identify: function () {
        var _identify = asyncToGenerator(
        /*#__PURE__*/
        regenerator.mark(function _callee(userId, traits, options, callback) {
          var id, data, opts, user, resolvedId;
          return regenerator.wrap(function _callee$(_context) {
            while (1) {
              switch (_context.prev = _context.next) {
                case 0:
                  id = a(userId) ? userId : null;
                  data = g(userId) ? userId : traits;
                  opts = options || {};
                  user = instance.user();
                  /* sets temporary in memory id. Not to be relied on */

                  f$2(tempKey(ID), id);
                  resolvedId = id || data.userId || getUserProp(ID, instance, data);
                  return _context.abrupt("return", new Promise(function (resolve) {
                    store.dispatch(objectSpread2$1({
                      type: EVENTS.identifyStart,
                      userId: resolvedId,
                      traits: data || {},
                      options: opts,
                      anonymousId: user.anonymousId
                    }, user.id && user.id !== id && {
                      previousId: user.id
                    }), resolve, [traits, options, callback]);
                  }));

                case 7:
                case "end":
                  return _context.stop();
              }
            }
          }, _callee);
        }));

        function identify(_x, _x2, _x3, _x4) {
          return _identify.apply(this, arguments);
        }

        return identify;
      }(),

      /**
       * Track an analytics event. This will trigger `track` calls in any installed plugins
       * @typedef {Function} Track
       * @param  {String}   eventName - Event name
       * @param  {Object}   [payload]   - Event payload
       * @param  {Object}   [options]   - Event options
       * @param  {Function} [callback]  - Callback to fire after tracking completes
       * @returns {Promise}
       * @api public
       *
       * @example
       *
       * // Basic event tracking
       * analytics.track('buttonClicked')
       *
       * // Event tracking with payload
       * analytics.track('itemPurchased', {
       *   price: 11,
       *   sku: '1234'
       * })
       *
       * // Fire callback with 2nd or 3rd argument
       * analytics.track('newsletterSubscribed', () => {
       *   console.log('do this after track')
       * })
       *
       * // Disable sending this event to specific analytic tools
       * analytics.track('cartAbandoned', {
       *   items: ['xyz', 'abc']
       * }, {
       *   plugins: {
       *     // disable track event for segment
       *     segment: false
       *   }
       * })
       *
       * // Send event to only to specific analytic tools
       * analytics.track('customerIoOnlyEventExample', {
       *   price: 11,
       *   sku: '1234'
       * }, {
       *   plugins: {
       *     // disable this specific track call all plugins except customerio
       *     all: false,
       *     customerio: true
       *   }
       * })
       */
      track: function () {
        var _track = asyncToGenerator(
        /*#__PURE__*/
        regenerator.mark(function _callee2(eventName, payload, options, callback) {
          var name, data, opts;
          return regenerator.wrap(function _callee2$(_context2) {
            while (1) {
              switch (_context2.prev = _context2.next) {
                case 0:
                  name = g(eventName) ? eventName.event : eventName;

                  if (!(!name || !a(name))) {
                    _context2.next = 3;
                    break;
                  }

                  throw new Error('EventMissing');

                case 3:
                  data = g(eventName) ? eventName : payload || {};
                  opts = g(options) ? options : {};
                  return _context2.abrupt("return", new Promise(function (resolve) {
                    store.dispatch({
                      type: EVENTS.trackStart,
                      event: name,
                      properties: data,
                      options: opts,
                      userId: getUserProp(ID, instance, payload),
                      anonymousId: getUserProp(ANONID, instance, payload)
                    }, resolve, [payload, options, callback]);
                  }));

                case 6:
                case "end":
                  return _context2.stop();
              }
            }
          }, _callee2);
        }));

        function track(_x5, _x6, _x7, _x8) {
          return _track.apply(this, arguments);
        }

        return track;
      }(),

      /**
       * Trigger page view. This will trigger `page` calls in any installed plugins
       * @typedef {Function} Page
       * @param  {PageData} [data] - Page data overrides.
       * @param  {Object}   [options] - Page tracking options
       * @param  {Function} [callback] - Callback to fire after page view call completes
       * @returns {Promise}
       * @api public
       *
       * @example
       *
       * // Basic page tracking
       * analytics.page()
       *
       * // Page tracking with page data overrides
       * analytics.page({
       *   url: 'https://google.com'
       * })
       *
       * // Fire callback with 1st, 2nd or 3rd argument
       * analytics.page(() => {
       *   console.log('do this after page')
       * })
       *
       * // Disable sending this pageview to specific analytic tools
       * analytics.page({}, {
       *   plugins: {
       *     // disable page tracking event for segment
       *     segment: false
       *   }
       * })
       *
       * // Send pageview to only to specific analytic tools
       * analytics.page({}, {
       *   plugins: {
       *     // disable this specific page in all plugins except customerio
       *     all: false,
       *     customerio: true
       *   }
       * })
       */
      page: function () {
        var _page = asyncToGenerator(
        /*#__PURE__*/
        regenerator.mark(function _callee3(data, options, callback) {
          var d$$1, opts;
          return regenerator.wrap(function _callee3$(_context3) {
            while (1) {
              switch (_context3.prev = _context3.next) {
                case 0:
                  d$$1 = g(data) ? data : {};
                  opts = g(options) ? options : {};
                  /*
                  // @TODO add custom value reolvers for userId and anonId
                  if (resolvers.getUserId) {
                    const asyncUserId = await resolvers.getUserId()
                    console.log('x', x)
                  }
                  */

                  return _context3.abrupt("return", new Promise(function (resolve) {
                    store.dispatch({
                      type: EVENTS.pageStart,
                      properties: getPageData(d$$1),
                      options: opts,
                      userId: getUserProp(ID, instance, d$$1),
                      anonymousId: getUserProp(ANONID, instance, d$$1)
                    }, resolve, [data, options, callback]);
                  }));

                case 3:
                case "end":
                  return _context3.stop();
              }
            }
          }, _callee3);
        }));

        function page$$1(_x9, _x10, _x11) {
          return _page.apply(this, arguments);
        }

        return page$$1;
      }(),

      /**
       * Get user data
       * @typedef {Function} User
       * @param {string} [key] - dot.prop.path of user data. Example: 'traits.company.name'
       * @returns {string|object} value of user data or null
       *
       * @example
       *
       * // Get all user data
       * const userData = analytics.user()
       *
       * // Get user id
       * const userId = analytics.user('userId')
       *
       * // Get user company name
       * const companyName = analytics.user('traits.company.name')
       */
      user: function user(key) {
        if (key === ID || key === 'id') {
          return getUserProp(ID, instance);
        }

        if (key === ANONID || key === 'anonId') {
          return getUserProp(ANONID, instance);
        }

        var user = instance.getState('user');
        if (!key) return user;
        return dotProp(user, key);
      },

      /**
       * Clear all information about the visitor & reset analytic state.
       * @typedef {Function} Reset
       * @param {Function} [callback] - Handler to run after reset
       * @returns {Promise}
       * @example
       *
       * // Reset current visitor
       * analytics.reset()
       */
      reset: function reset(callback) {
        return new Promise(function (resolve) {
          store.dispatch({
            type: EVENTS.resetStart
          }, resolve, callback);
        });
      },

      /**
       * Fire callback on analytics ready event
       * @typedef {Function} Ready
       * @param  {Function} callback - function to trigger when all providers have loaded
       * @returns {DetachListeners} - Function to detach listener
       *
       * @example
       *
       * analytics.ready() => {
       *   console.log('all plugins have loaded or were skipped', payload)
       * })
       */
      ready: function ready(callback) {
        return instance.on(EVENTS.ready, callback);
      },

      /**
       * Attach an event handler function for analytics lifecycle events.
       * @typedef {Function} On
       * @param  {String}   name - Name of event to listen to
       * @param  {Function} callback - function to fire on event
       * @return {DetachListeners} - Function to detach listener
       *
       * @example
       *
       * // Fire function when 'track' calls happen
       * analytics.on('track', ({ payload }) => {
       *   console.log('track call just happened. Do stuff')
       * })
       *
       * // Remove listener before it is called
       * const removeListener = analytics.on('track', ({ payload }) => {
       *   console.log('This will never get called')
       * })
       *
       * // cleanup .on listener
       * removeListener()
       */
      on: function on(name, callback) {
        if (!name || !p(callback)) {
          return false;
        }

        if (name === EVENTS.bootstrap) {
          throw new Error('.on disabled for ' + name);
        }

        var startRegex = /Start$|Start:/;

        if (name === '*') {
          var beforeHandler = function beforeHandler(store) {
            return function (next) {
              return function (action) {
                if (action.type.match(startRegex)) {
                  callback({
                    // eslint-disable-line
                    payload: action,
                    instance: instance,
                    plugins: customPlugins
                  });
                }

                return next(action);
              };
            };
          };

          var afterHandler = function afterHandler(store) {
            return function (next) {
              return function (action) {
                if (!action.type.match(startRegex)) {
                  callback({
                    // eslint-disable-line
                    payload: action,
                    instance: instance,
                    plugins: customPlugins
                  });
                }

                return next(action);
              };
            };
          };

          addMiddleware(beforeHandler, before);
          addMiddleware(afterHandler, after);
          /**
           * Detach listeners
           * @typedef {Function} DetachListeners
           */

          return function () {
            removeMiddleware(beforeHandler, before);
            removeMiddleware(afterHandler, after);
          };
        }

        var position = name.match(startRegex) ? before : after; // eslint-disable-line

        var handler = function handler(store) {
          return function (next) {
            return function (action) {
              // Subscribe to EVERYTHING
              if (action.type === name) {
                callback({
                  // eslint-disable-line
                  payload: action,
                  instance: instance,
                  plugins: customPlugins,
                  abort: nonAbortable
                });
              }
              /* For future matching of event subpaths `track:*` etc
              } else if (name.match(/\*$/)) {
                const match = (name === '*') ? '.' : name
                const regex = new RegExp(`${match}`, 'g')
              } */


              return next(action);
            };
          };
        };

        addMiddleware(handler, position);
        return function () {
          return removeMiddleware(handler, position);
        };
      },

      /**
       * Attach a handler function to an event and only trigger it only once.
       * @typedef {Function} Once
       * @param  {String} name - Name of event to listen to
       * @param  {Function} callback - function to fire on event
       * @return {DetachListeners} - Function to detach listener
       *
       * @example
       *
       * // Fire function only once 'track'
       * analytics.once('track', ({ payload }) => {
       *   console.log('This will only triggered once when analytics.track() fires')
       * })
       *
       * // Remove listener before it is called
       * const listener = analytics.once('track', ({ payload }) => {
       *   console.log('This will never get called b/c listener() is called')
       * })
       *
       * // cleanup .once listener before it fires
       * listener()
       */
      once: function once(name, callback) {
        if (!name || !p(callback)) {
          return false;
        }

        if (name === EVENTS.bootstrap) {
          throw new Error('.once disabled for ' + name);
        }

        var detachListener = instance.on(name, function (_ref2) {
          var payload = _ref2.payload;
          callback({
            // eslint-disable-line
            payload: payload,
            instance: instance,
            plugins: customPlugins,
            abort: nonAbortable
          }); // detach listener after its called once

          detachListener();
        });
        return detachListener;
      },

      /**
       * Get data about user, activity, or context. Access sub-keys of state with `dot.prop` syntax.
       * @typedef {Function} GetState
       * @param  {string} [key] - dot.prop.path value of state
       * @return {any}
       *
       * @example
       *
       * // Get the current state of analytics
       * analytics.getState()
       *
       * // Get a subpath of state
       * analytics.getState('context.offline')
       */
      getState: function getState(key) {
        var state = store.getState();
        if (key) return dotProp(state, key);
        return Object.assign({}, state);
      },

      /*
       * Emit events for other plugins or middleware to react to.
       * @param  {Object} action - event to dispatch
       */
      dispatch: function dispatch(action) {
        var actionData = a(action) ? {
          type: action
        } : action;

        if (isReservedAction(actionData.type)) {
          throw new Error('reserved action ' + actionData.type);
        }

        var _private = action._ || {}; // Dispatch actionStart
        // const autoPrefixType = `${actionData.type.replace(/Start$/, '')}Start`


        var dispatchData = objectSpread2$1(objectSpread2$1({}, actionData), {}, {
          _: objectSpread2$1({
            originalAction: actionData.type
          }, _private) // type: `${autoPrefixType}`

        });
        store.dispatch(dispatchData);
      },
      // Do not use. Will be removed. Here for Backwards compatiblity.
      // Moved to analytics.plugins.enable
      enablePlugin: plugins.enable,
      /// Do not use. Will be removed. Here for Backwards compatiblity.
      /// Moved to analytics.plugins.disable
      disablePlugin: plugins.disable,
      // Do not use. Will be removed. Here for Backwards compatiblity.
      // New plugins api
      plugins: plugins,

      /**
       * Storage utilities for persisting data.
       * These methods will allow you to save data in localStorage, cookies, or to the window.
       * @typedef {Object} Storage
       * @property {GetItem} getItem - Get value from storage
       * @property {SetItem} setItem - Set storage value
       * @property {RemoveItem} removeItem - Remove storage value
       *
       * @example
       *
       * // Pull storage off analytics instance
       * const { storage } = analytics
       *
       * // Get value
       * storage.getItem('storage_key')
       *
       * // Set value
       * storage.setItem('storage_key', 'value')
       *
       * // Remove value
       * storage.removeItem('storage_key')
       */
      storage: {
        /**
         * Get value from storage
         * @typedef {Function} GetItem
         * @param {String} key - storage key
         * @param {Object} [options] - storage options
         * @return {Any}
         *
         * @example
         *
         * analytics.storage.getItem('storage_key')
         */
        getItem: storage.getItem,

        /**
         * Set storage value
         * @typedef {Function} SetItem
         * @param {String} key - storage key
         * @param {any} value - storage value
         * @param {Object} [options] - storage options
         *
         * @example
         *
         * analytics.storage.setItem('storage_key', 'value')
         */
        setItem: function setItem(key, value, options) {
          store.dispatch({
            type: EVENTS.setItemStart,
            key: key,
            value: value,
            options: options
          });
        },

        /**
         * Remove storage value
         * @typedef {Function} RemoveItem
         * @param {String} key - storage key
         * @param {Object} [options] - storage options
         *
         * @example
         *
         * analytics.storage.removeItem('storage_key')
         */
        removeItem: function removeItem(key, options) {
          store.dispatch({
            type: EVENTS.removeItemStart,
            key: key,
            options: options
          });
        }
      },

      /*
       * Set the anonymous ID of the visitor
       * @param {String} anonymousId - anonymous Id to set
       * @param {Object} [options] - storage options
       *
       * @example
       *
       * // Set anonymous ID
       * analytics.setAnonymousId('1234567')
       */
      setAnonymousId: function setAnonymousId(anonymousId, options) {
        /* sets temporary in memory id. Not to be relied on */
        // set(tempKey(ANONID), anonymousId)
        instance.storage.setItem(ANON_ID, anonymousId, options);
      },

      /*
       * Events exposed by core analytics library and all loaded plugins
       * @type {Array}
       */
      events: {
        all: allSystemEvents,
        core: coreEvents,
        plugins: allPluginEvents // byType: (type) => {} @Todo grab logic from engine and give inspectable events

      }
    };

    var enrichMiddleware = function enrichMiddleware(storeAPI) {
      return function (next) {
        return function (action) {
          if (!action.meta) {
            action.meta = generateMeta();
          }

          return next(action);
        };
      };
    };

    var middlewares = parsedOptions.middlewares.concat([enrichMiddleware,
    /* Core analytics middleware */
    dynamicMiddlewares(before), // Before dynamic middleware <-- fixed pageStart .on listener

    /* Plugin engine */
    pluginMiddleware(instance, getPlugins, {
      all: allSystemEvents,
      plugins: allPluginEvents
    }), storageMiddleware(storage), initializeMiddleware(instance), identifyMiddleware(instance, storage),
    /* after dynamic middleware */
    dynamicMiddlewares(after)]);
    /* Initial analytics state keys */

    var coreReducers = {
      context: context,
      user: userReducer(storage),
      page: page,
      track: trackReducer,
      plugins: createReducer(getPlugins),
      queue: queueReducer
    };
    var composeEnhancers = compose;
    var composeWithGlobalDebug = compose;

    if (c && config.debug) {
      var devTools = window.__REDUX_DEVTOOLS_EXTENSION_COMPOSE__;

      if (devTools) {
        composeEnhancers = devTools({
          trace: true,
          traceLimit: 25
        });
      }

      composeWithGlobalDebug = function composeWithGlobalDebug() {
        if (arguments.length === 0) return Debug();
        if (g(_typeof_1(arguments[0]))) return composeWithDebug(arguments[0]);
        return composeWithDebug().apply(null, arguments);
      };
    }

    var initialConfig = makeContext(config);
    var intialPluginState = parsedOptions.pluginsArray.reduce(function (acc, plugin) {
      var name = plugin.name,
          config = plugin.config,
          loaded = plugin.loaded;
      var isEnabled = parsedOptions.pluginEnabled[name];
      acc[name] = {
        enabled: isEnabled,
        // If plugin enabled & has no initialize method, set initialized to true, else false
        initialized: isEnabled ? Boolean(!plugin.initialize) : false,
        loaded: Boolean(loaded()),
        config: config || {}
      };
      return acc;
    }, {});
    var initialState = {
      context: initialConfig,
      user: visitorInfo,
      plugins: intialPluginState // Todo allow for more userland defined initial state?

      /* Create analytics store! */

    };
    var store = createStore( // register reducers
    combineReducers(objectSpread2$1(objectSpread2$1({}, coreReducers), customReducers)), // set user defined initial state
    initialState, // register middleware & plugins used
    composeWithGlobalDebug(composeEnhancers(applyMiddleware.apply(void 0, toConsumableArray(middlewares)))));
    /* Supe up dispatch with callback promise resolver. Happens in enrichMeta */

    function enhanceDispatch(fn) {
      return function (event, resolver, callbacks) {
        // console.log('original event', event)
        var meta = generateMeta(event.meta, resolver, ensureArray(callbacks)); // if (resolver) console.log('dispatch resolver', resolver)
        // if (callbacks) console.log('dispatch callbacks', callbacks)

        var newEvent = objectSpread2$1(objectSpread2$1({}, event), {
          meta: meta
        }); // console.log('newEvent', newEvent)

        return fn.apply(null, [newEvent]);
      };
    } // Automatically apply meta to dispatch calls


    store.dispatch = enhanceDispatch(store.dispatch);
    /* Synchronously call bootstrap & register Plugin methods */

    var pluginKeys = Object.keys(customPlugins);
    /* Bootstrap analytic plugins */

    store.dispatch({
      type: EVENTS.bootstrap,
      plugins: pluginKeys,
      config: initialConfig,
      params: params,
      user: visitorInfo,
      initialUser: initialUser,
      persistedUser: persistedUser
    });
    var enabledPlugins = pluginKeys.filter(function (name) {
      return parsedOptions.pluginEnabled[name];
    });
    var disabledPlugins = pluginKeys.filter(function (name) {
      return !parsedOptions.pluginEnabled[name];
    });
    /* Register analytic plugins */

    store.dispatch({
      type: EVENTS.registerPlugins,
      plugins: pluginKeys,
      enabled: parsedOptions.pluginEnabled
    });
    /* dispatch register for individual plugins */

    parsedOptions.pluginsArray.map(function (plugin, i$$1) {
      var bootstrap = plugin.bootstrap,
          config = plugin.config,
          name = plugin.name;

      if (bootstrap && p(bootstrap)) {
        bootstrap({
          instance: instance,
          config: config,
          payload: plugin
        });
      }
      /* Register plugins */


      store.dispatch({
        type: EVENTS.registerPluginType(name),
        name: name,
        enabled: parsedOptions.pluginEnabled[name],
        plugin: plugin
      });
      /* All plugins registered initialize, is last loop */

      if (parsedOptions.pluginsArray.length === i$$1 + 1) {
        store.dispatch({
          type: EVENTS.initializeStart,
          plugins: enabledPlugins,
          disabled: disabledPlugins
        });
      }
    });

    function appendArguments(fn) {
      return function () {
        /* Get original args */
        var args = Array.prototype.slice.call(arguments);
        /* Create clone of args */

        var newArgs = new Array(fn.length);

        for (var i$$1 = 0; i$$1 < args.length; i$$1++) {
          newArgs[i$$1] = args[i$$1];
        }
        /* Append new arg to end */


        newArgs[newArgs.length] = instance; // Set instance on extended methods

        return fn.apply({
          instance: instance
        }, newArgs);
      };
    }
    /* Return analytics instance */


    return instance;
  } // Duplicated strings


  var before = 'before';
  var after = 'after';

  var i$2 = a$2(),
      r$1 = p$2,
      c$2 = p$2;

  function u$2(e) {
    return i$2 ? p$2(e, "", -1) : n$2(e);
  }

  function a$2() {
    if (typeof i$2 !== e$1) return i$2;

    try {
      p$2(e$1, "1"), i$2 = -1 !== document.cookie.indexOf(e$1), u$2(e$1);
    } catch (e) {
      i$2 = !1;
    }

    return i$2;
  }

  function p$2(t, r, c, u, a, p) {
    if (typeof window !== e$1) {
      var d = arguments.length > 1;
      return i$2 || (d ? f$2(t, r) : o$2(t)), d ? document.cookie = t + "=" + encodeURIComponent(r) + (c ? "; expires=" + new Date(+new Date() + 1e3 * c).toUTCString() + (u ? "; path=" + u : "") + (a ? "; domain=" + a : "") + (p ? "; secure" : "") : "") : decodeURIComponent((("; " + document.cookie).split("; " + t + "=")[1] || "").split(";")[0]);
    }
  }

  var a$3 = l$3();

  function l$3() {
    if (typeof a$3 !== e$1) return a$3;
    a$3 = !0;

    try {
      typeof localStorage !== e$1 && typeof JSON !== e$1 || (a$3 = !1), localStorage.setItem(e$1, e$1), localStorage.removeItem(e$1);
    } catch (t) {
      a$3 = !1;
    }

    return a$3;
  }

  function s$2(o$$1) {
    var t$$1 = o$$1;

    try {
      if ("true" === (t$$1 = JSON.parse(o$$1))) return !0;
      if ("false" === t$$1) return !1;
      if (g(t$$1)) return t$$1;
      parseFloat(t$$1) === t$$1 && (t$$1 = parseFloat(t$$1));
    } catch (o$$1) {}

    if (null !== t$$1 && "" !== t$$1) return t$$1;
  }

  var k = l$3(),
      y$2 = a$2();

  function b$2(t$$1, r$$1) {
    if (t$$1) {
      var e$$1 = C(r$$1),
          l$$1 = !F(e$$1),
          i$$1 = d$2(e$$1) ? s$2(localStorage.getItem(t$$1)) : void 0;
      if (l$$1 && !l(i$$1)) return i$$1;
      var c$$1 = h$1(e$$1) ? s$2(r$1(t$$1)) : void 0;
      if (l$$1 && c$$1) return c$$1;
      var n$$1 = o$2(t$$1);
      return l$$1 ? n$$1 : {
        localStorage: i$$1,
        cookie: c$$1,
        global: n$$1
      };
    }
  }

  function I(r$$1, e$$1, i$$1) {
    if (r$$1 && !l(e$$1)) {
      var c$$1 = {},
          n$$1 = C(i$$1),
          g$$1 = JSON.stringify(e$$1),
          f$$1 = !F(n$$1);
      return d$2(n$$1) && (c$$1.localStorage = {
        location: "localStorage",
        current: e$$1,
        previous: s$2(localStorage.getItem(r$$1))
      }, localStorage.setItem(r$$1, g$$1), f$$1) ? c$$1.localStorage : h$1(n$$1) && (c$$1.cookie = {
        location: "cookie",
        current: e$$1,
        previous: s$2(r$1(r$$1))
      }, c$2(r$$1, g$$1), f$$1) ? c$$1.cookie : (c$$1.global = {
        location: "global",
        current: e$$1,
        previous: o$2(r$$1)
      }, f$2(r$$1, e$$1), f$$1 ? c$$1.global : c$$1);
    }
  }

  function x$1(o$$1, t$$1) {
    if (o$$1) {
      var e$$1 = C(t$$1),
          a$$1 = b$2(o$$1, "*"),
          l$$1 = {};
      return !l(a$$1.localStorage) && d$2(e$$1) && (localStorage.removeItem(o$$1), l$$1.localStorage = a$$1.localStorage), !l(a$$1.cookie) && h$1(e$$1) && (u$2(o$$1), l$$1.cookie = a$$1.cookie), !l(a$$1.global) && J(e$$1, "global") && (n$2(o$$1), l$$1.global = a$$1.global), l$$1;
    }
  }

  function C(o$$1) {
    return o$$1 ? a(o$$1) ? o$$1 : o$$1.storage : "any";
  }

  function d$2(o$$1) {
    return k && J(o$$1, "localStorage");
  }

  function h$1(o$$1) {
    return y$2 && J(o$$1, "cookie");
  }

  function F(o$$1) {
    return "*" === o$$1 || "all" === o$$1;
  }

  function J(o$$1, t$$1) {
    return "any" === o$$1 || o$$1 === t$$1 || F(o$$1);
  }

  var N = {
    setItem: I,
    getItem: b$2,
    removeItem: x$1
  };

  function analyticsLib() {
    var opts = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
    var defaultSettings = {
      storage: N
    };
    return analytics(objectSpread2(objectSpread2({}, defaultSettings), opts));
  }

  exports.default = analyticsLib;
  exports.init = analyticsLib;
  exports.Analytics = analyticsLib;
  exports.EVENTS = EVENTS;
  exports.CONSTANTS = constants;

  return exports;

}({}));
