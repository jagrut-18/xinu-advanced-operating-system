// #define number_inputs (sizeof (stream_input) / sizeof (const char *))
// const char *stream_input[] = {\
// "8\t6\t78",\
// "0\t11\t53",\
// "3\t19\t12",\
// "9\t22\t29",\
// "6\t30\t19",\
// "6\t36\t84",\
// "2\t36\t18",\
// "0\t43\t42",\
// "8\t44\t96",\
// "0\t51\t33",\
// "1\t57\t24",\
// "3\t57\t75",\
// "7\t65\t41",\
// "2\t71\t70",\
// "0\t80\t41",\
// "3\t87\t62",\
// "6\t87\t96",\
// "1\t87\t61",\
// "3\t88\t70",\
// "5\t96\t48",\
// "6\t100\t93",\
// "4\t105\t61",\
// "0\t107\t2",\
// "2\t113\t25",\
// "1\t118\t36",\
// "2\t125\t32",\
// "9\t125\t52",\
// "1\t126\t51",\
// "2\t135\t63",\
// "2\t141\t50",\
// "2\t146\t63",\
// "0\t152\t67",\
// "2\t157\t37",\
// "2\t165\t13",\
// "9\t166\t16",\
// "9\t175\t5",\
// "8\t176\t70",\
// "0\t177\t46",\
// "8\t177\t18",\
// "4\t184\t26",\
// "1\t184\t2",\
// "2\t190\t35",\
// "8\t191\t42",\
// "0\t198\t61",\
// "8\t202\t16",\
// "5\t203\t24",\
// "8\t206\t79",\
// "2\t211\t74",\
// "1\t219\t45",\
// "6\t227\t72",\
// "1\t233\t4",\
// "8\t238\t64",\
// "9\t245\t50",\
// "7\t254\t76",\
// "9\t254\t35",\
// "7\t254\t7",\
// "7\t257\t34",\
// "1\t259\t53",\
// "7\t261\t24",\
// "7\t270\t62",\
// "4\t279\t99",\
// "6\t279\t89",\
// "9\t286\t36",\
// "9\t295\t24",\
// "9\t300\t65",\
// "9\t305\t23",\
// "7\t306\t20",\
// "4\t309\t41",\
// "1\t316\t74",\
// "9\t325\t26",\
// "9\t327\t46",\
// "0\t333\t0",\
// "0\t334\t11",\
// "6\t339\t30",\
// "7\t345\t61",\
// "6\t353\t60",\
// "9\t353\t4",\
// "5\t360\t78",\
// "0\t360\t57",\
// "9\t366\t16",\
// "8\t370\t9",\
// "2\t371\t18",\
// "6\t375\t48",\
// "7\t376\t87",\
// "9\t377\t92",\
// "2\t378\t93",\
// "3\t385\t37",\
// "7\t394\t79",\
// "4\t398\t95",\
// "5\t403\t35",\
// "2\t404\t88",\
// "3\t405\t67",\
// "5\t414\t50",\
// "8\t421\t66",\
// "3\t425\t30",\
// "4\t428\t67",\
// "2\t435\t43",\
// "9\t442\t18",\
// "8\t445\t24",\
// "3\t452\t42",\
// "7\t453\t41",\
// "4\t456\t9",\
// "5\t462\t48",\
// "4\t463\t56",\
// "5\t470\t19",\
// "3\t476\t84",\
// "1\t484\t96",\
// "6\t487\t10",\
// "6\t492\t31",\
// "1\t494\t87",\
// "4\t496\t3",\
// "3\t503\t39",\
// "4\t508\t98",\
// "5\t516\t57",\
// "4\t521\t11",\
// "1\t529\t9",\
// "8\t530\t12",\
// "6\t530\t52",\
// "5\t533\t42",\
// "2\t533\t34",\
// "4\t535\t77",\
// "0\t535\t75",\
// "8\t544\t46",\
// "3\t552\t38",\
// "3\t554\t11",\
// "2\t554\t90",\
// "6\t554\t79",\
// "9\t554\t7",\
// "3\t557\t97",\
// "5\t557\t86",\
// "4\t562\t65",\
// "3\t565\t65",\
// "4\t572\t29",\
// "8\t573\t12",\
// "5\t577\t19",\
// "8\t585\t54",\
// "5\t592\t35",\
// "9\t596\t77",\
// "0\t604\t70",\
// "1\t606\t45",\
// "4\t615\t83",\
// "5\t622\t68",\
// "0\t626\t40",\
// "3\t633\t4",\
// "8\t633\t32",\
// "8\t637\t26",\
// "6\t642\t41",\
// "2\t646\t3",\
// "1\t649\t58",\
// "9\t654\t98",\
// "8\t662\t76",\
// "8\t668\t11",\
// "5\t677\t3",\
// "5\t679\t9",\
// "5\t686\t17",\
// "6\t686\t52",\
// "2\t694\t1",\
// "9\t696\t89",\
// "8\t697\t57",\
// "9\t703\t65",\
// "4\t708\t22",\
// "9\t714\t0",\
// "4\t718\t19",\
// "6\t724\t93",\
// "2\t729\t7",\
// "3\t729\t35",\
// "4\t730\t54",\
// "6\t737\t56",\
// "8\t738\t7",\
// "2\t747\t62",\
// "1\t754\t93",\
// "1\t754\t47",\
// "6\t755\t78",\
// "0\t761\t70",\
// "0\t770\t42",\
// "6\t779\t28",\
// "7\t780\t34",\
// "9\t784\t42",\
// "6\t789\t18",\
// "9\t792\t14",\
// "9\t801\t0",\
// "8\t803\t11",\
// "6\t809\t72",\
// "3\t813\t21",\
// "4\t819\t46",\
// "9\t822\t52",\
// "4\t829\t78",\
// "8\t833\t64",\
// "8\t841\t35",\
// "2\t849\t41",\
// "7\t856\t36",\
// "2\t863\t36",\
// "1\t872\t28",\
// "0\t877\t34",\
// "0\t883\t17",\
// "8\t887\t21",\
// "8\t891\t56",\
// "6\t894\t6",\
// "9\t900\t16",\
// "8\t901\t86",\
// "6\t908\t76",\
// "6\t912\t81",\
// "7\t913\t44",\
// "9\t913\t50",\
// "9\t917\t94",\
// "8\t920\t90",\
// "0\t924\t68",\
// "7\t924\t6",\
// "7\t929\t48",\
// "5\t933\t19",\
// "8\t937\t82",\
// "6\t942\t37",\
// "5\t943\t40",\
// "7\t947\t64",\
// "9\t955\t52",\
// "7\t955\t15",\
// "3\t958\t82",\
// "2\t967\t27",\
// "5\t970\t57",\
// "5\t977\t10",\
// "9\t984\t81",\
// "2\t985\t57",\
// "6\t991\t19",\
// "7\t998\t33",\
// "1\t1006\t78",\
// "0\t1007\t47",\
// "1\t1011\t27",\
// "4\t1011\t95",\
// "4\t1019\t63",\
// "2\t1023\t84",\
// "0\t1025\t74",\
// "1\t1030\t33",\
// "9\t1037\t96",\
// "5\t1045\t11",\
// "4\t1047\t93",\
// "7\t1051\t50",\
// "2\t1055\t63",\
// "4\t1062\t11",\
// "6\t1066\t24",\
// "9\t1066\t88",\
// "2\t1070\t26",\
// "1\t1079\t22",\
// "1\t1079\t69",\
// "5\t1088\t93",\
// "6\t1090\t0",\
// "9\t1094\t87",\
// "8\t1102\t29",\
// "9\t1102\t23",\
// "7\t1111\t56",\
// "0\t1118\t59",\
// "1\t1127\t77",\
// "2\t1136\t44",\
// "1\t1137\t52",\
// "9\t1143\t73",\
// "4\t1146\t87",\
// "6\t1148\t5",\
// "7\t1155\t53",\
// "0\t1159\t1",\
// "0\t1160\t66",\
// "0\t1167\t12",\
// "2\t1169\t44",\
// "6\t1176\t77",\
// "8\t1177\t64",\
// "5\t1185\t89",\
// "9\t1190\t56",\
// "6\t1194\t73",\
// "3\t1194\t9",\
// "7\t1199\t72",\
// "1\t1200\t84",\
// "4\t1207\t12",\
// "5\t1213\t12",\
// "4\t1220\t79",\
// "7\t1222\t68",\
// "8\t1222\t65",\
// "4\t1223\t27",\
// "7\t1226\t15",\
// "4\t1235\t62",\
// "9\t1243\t26",\
// "0\t1248\t44",\
// "5\t1254\t57",\
// "5\t1257\t58",\
// "7\t1263\t97",\
// "1\t1264\t54",\
// "5\t1267\t36",\
// "3\t1274\t1",\
// "0\t1277\t79",\
// "9\t1286\t73",\
// "5\t1291\t83",\
// "3\t1294\t0",\
// "5\t1298\t67",\
// "7\t1301\t34",\
// "0\t1302\t48",\
// "4\t1302\t95",\
// "5\t1307\t58",\
// "2\t1311\t97",\
// "1\t1320\t72",\
// "4\t1324\t86",\
// "6\t1325\t69",\
// "7\t1329\t57",\
// "2\t1336\t27",\
// "7\t1344\t33",\
// "2\t1348\t20",\
// "8\t1348\t1",\
// "2\t1350\t95",\
// "6\t1358\t13",\
// "5\t1363\t13",\
// "7\t1371\t41",\
// "2\t1377\t15",\
// "8\t1386\t27",\
// "9\t1391\t87",\
// "3\t1400\t53",\
// "9\t1403\t82",\
// "3\t1409\t67",\
// "9\t1416\t45",\
// "7\t1418\t93",\
// "7\t1426\t34",\
// "7\t1435\t40",\
// "3\t1435\t44",\
// "6\t1438\t63",\
// "5\t1439\t85",\
// "6\t1441\t2",\
// "0\t1447\t3",\
// "0\t1455\t40",\
// "3\t1461\t5",\
// "5\t1463\t62",\
// "7\t1466\t95",\
// "8\t1472\t71",\
// "4\t1474\t81",\
// "4\t1475\t91",\
// "5\t1484\t69",\
// "1\t1491\t88",\
// "5\t1493\t64",\
// "6\t1495\t50",\
// "4\t1501\t88",\
// "2\t1509\t90",\
// "6\t1516\t87",\
// "5\t1517\t35",\
// "4\t1518\t0",\
// "2\t1524\t85",\
// "5\t1529\t33",\
// "7\t1534\t94",\
// "0\t1534\t40",\
// "8\t1541\t82",\
// "1\t1545\t72",\
// "8\t1545\t82",\
// "0\t1550\t14",\
// "3\t1550\t38",\
// "2\t1550\t13",\
// "8\t1558\t10",\
// "6\t1564\t34",\
// "2\t1572\t5",\
// "3\t1573\t28",\
// "9\t1578\t28",\
// "3\t1579\t7",\
// "9\t1585\t62",\
// "3\t1586\t48",\
// "4\t1594\t94",\
// "1\t1602\t16",\
// "0\t1609\t34",\
// "0\t1615\t45",\
// "1\t1617\t34",\
// "3\t1622\t82",\
// "0\t1625\t5",\
// "1\t1633\t72",\
// "9\t1639\t22",\
// "6\t1644\t7",\
// "9\t1644\t99",\
// "8\t1648\t3",\
// "5\t1655\t38",\
// "1\t1655\t52",\
// "3\t1664\t91",\
// "1\t1670\t20",\
// "5\t1671\t99",\
// "0\t1675\t28",\
// "4\t1677\t23",\
// "1\t1683\t64",\
// "0\t1688\t37",\
// "3\t1688\t21",\
// "4\t1694\t59",\
// "1\t1694\t64",\
// "0\t1697\t8",\
// "9\t1697\t98",\
// "4\t1697\t10",\
// "6\t1701\t43",\
// "7\t1703\t56",\
// "4\t1710\t25",\
// "0\t1716\t95",\
// "6\t1721\t12",\
// "1\t1727\t98",\
// "6\t1735\t58",\
// "7\t1741\t77",\
// "1\t1748\t94",\
// "5\t1754\t77",\
// "1\t1757\t76",\
// "9\t1761\t13",\
// "4\t1765\t42",\
// "7\t1773\t14",\
// "2\t1775\t4",\
// "1\t1779\t90",\
// "8\t1784\t78",\
// "8\t1784\t0",\
// "6\t1792\t71",\
// "4\t1797\t9",\
// "1\t1804\t52",\
// "3\t1808\t92",\
// "0\t1811\t38",\
// "8\t1814\t68",\
// "6\t1814\t63",\
// "7\t1818\t11",\
// "1\t1818\t28",\
// "1\t1823\t30",\
// "3\t1829\t66",\
// "1\t1834\t88",\
// "6\t1838\t0",\
// "5\t1843\t36",\
// "4\t1850\t38",\
// "4\t1852\t76",\
// "9\t1857\t53",\
// "2\t1861\t9",\
// "0\t1866\t15",\
// "1\t1867\t81",\
// "9\t1874\t84",\
// "0\t1883\t9",\
// "5\t1892\t30",\
// "0\t1895\t86",\
// "5\t1902\t31",\
// "8\t1903\t67",\
// "2\t1911\t42",\
// "3\t1917\t20",\
// "9\t1923\t20",\
// "7\t1927\t46",\
// "3\t1933\t76",\
// "2\t1941\t22",\
// "0\t1947\t88",\
// "0\t1956\t7",\
// "1\t1958\t80",\
// "9\t1958\t27",\
// "6\t1963\t92",\
// "5\t1966\t82",\
// "8\t1970\t31",\
// "8\t1973\t56",\
// "8\t1980\t41",\
// "7\t1980\t61",\
// "8\t1989\t22",\
// "8\t1989\t9",\
// "4\t1995\t28",\
// "8\t2004\t17",\
// "9\t2008\t24",\
// "3\t2012\t68",\
// "7\t2012\t96",\
// "1\t2014\t63",\
// "9\t2019\t78",\
// "7\t2019\t49",\
// "8\t2025\t21",\
// "8\t2032\t70",\
// "2\t2032\t71",\
// "9\t2036\t13",\
// "3\t2045\t99",\
// "4\t2046\t16",\
// "8\t2047\t31",\
// "1\t2049\t11",\
// "0\t2049\t25",\
// "7\t2058\t26",\
// "1\t2058\t59",\
// "5\t2059\t49",\
// "5\t2066\t94",\
// "3\t2067\t71",\
// "9\t2076\t8",\
// "6\t2084\t42",\
// "9\t2087\t54",\
// "4\t2087\t7",\
// "9\t2092\t4",\
// "2\t2101\t3",\
// "9\t2105\t83",\
// "1\t2111\t67",\
// "3\t2113\t50",\
// "3\t2122\t21",\
// "3\t2130\t17",\
// "0\t2135\t32",\
// "3\t2135\t29",\
// "3\t2136\t15",\
// "7\t2138\t40",\
// "8\t2138\t67",\
// "2\t2143\t67",\
// "1\t2144\t24",\
// "1\t2144\t76",\
// "2\t2149\t46",\
// "0\t2158\t1",\
// "3\t2163\t96",\
// "7\t2166\t72",\
// "9\t2170\t9",\
// "2\t2174\t44",\
// "6\t2177\t74",\
// "8\t2180\t98",\
// "7\t2184\t28",\
// "8\t2190\t61",\
// "7\t2196\t80",\
// "5\t2198\t83",\
// "9\t2206\t82",\
// "5\t2208\t73",\
// "4\t2211\t98",\
// "7\t2211\t22",\
// "1\t2212\t37",\
// "9\t2212\t88",\
// "3\t2212\t76",\
// "9\t2216\t44",\
// "2\t2221\t83",\
// "7\t2228\t89",\
// "1\t2233\t69",\
// "6\t2237\t23",\
// "7\t2242\t59",\
// "0\t2245\t63",\
// "4\t2249\t29",\
// "9\t2257\t32",\
// "5\t2263\t29",\
// "4\t2268\t77",\
// "9\t2275\t44",\
// "7\t2275\t66",\
// "5\t2275\t70",\
// "7\t2276\t83",\
// "4\t2281\t27",\
// "4\t2282\t73",\
// "1\t2283\t58",\
// "3\t2292\t55",\
// "9\t2294\t30",\
// "9\t2296\t73",\
// "5\t2297\t94",\
// "2\t2302\t57",\
// "6\t2306\t71",\
// "6\t2310\t41",\
// "6\t2311\t75",\
// "9\t2311\t72",\
// "7\t2315\t20",\
// "3\t2324\t82",\
// "9\t2325\t15",\
// "3\t2333\t13",\
// "6\t2335\t0",\
// "2\t2335\t19",\
// "2\t2341\t47",\
// "1\t2344\t56",\
// "1\t2350\t93",\
// "6\t2351\t87",\
// "1\t2351\t22",\
// "7\t2352\t56",\
// "2\t2359\t16",\
// "9\t2361\t29",\
// "3\t2364\t14",\
// "1\t2373\t8",\
// "2\t2375\t22",\
// "5\t2377\t80",\
// "2\t2382\t6",\
// "6\t2388\t42",\
// "9\t2391\t50",\
// "9\t2392\t36",\
// "5\t2393\t64",\
// "0\t2402\t19",\
// "2\t2404\t27",\
// "9\t2407\t38",\
// "4\t2408\t38",\
// "4\t2415\t47",\
// "6\t2421\t41",\
// "7\t2429\t8",\
// "0\t2432\t35",\
// "4\t2437\t98",\
// "7\t2441\t76",\
// "4\t2448\t2",\
// "7\t2456\t54",\
// "5\t2460\t58",\
// "9\t2463\t30",\
// "0\t2467\t15",\
// "4\t2473\t15",\
// "7\t2479\t67",\
// "5\t2486\t79",\
// "8\t2488\t91",\
// "0\t2490\t73",\
// "6\t2499\t6",\
// "4\t2505\t4",\
// "1\t2508\t6",\
// "7\t2516\t5",\
// "1\t2520\t94",\
// "1\t2529\t40",\
// "6\t2534\t41",\
// "0\t2540\t46",\
// "7\t2545\t31",\
// "8\t2551\t87",\
// "2\t2553\t14",\
// "9\t2556\t91",\
// "0\t2561\t71",\
// "9\t2570\t29",\
// "4\t2572\t61",\
// "1\t2573\t66",\
// "9\t2578\t57",\
// "4\t2579\t33",\
// "5\t2582\t22",\
// "9\t2583\t59",\
// "0\t2584\t59",\
// "4\t2592\t10",\
// "0\t2599\t42",\
// "3\t2603\t47",\
// "2\t2608\t56",\
// "9\t2614\t6",\
// "7\t2622\t94",\
// "4\t2622\t40",\
// "3\t2630\t71",\
// "6\t2634\t17",\
// "3\t2636\t66",\
// "5\t2642\t17",\
// "0\t2644\t11",\
// "5\t2648\t39",\
// "6\t2654\t9",\
// "4\t2663\t20",\
// "3\t2672\t88",\
// "9\t2679\t19",\
// "4\t2680\t49",\
// "8\t2680\t62",\
// "9\t2688\t90",\
// "1\t2688\t65",\
// "4\t2697\t15",\
// "0\t2701\t64",\
// "6\t2702\t6",\
// "3\t2702\t79",\
// "0\t2703\t72",\
// "2\t2705\t49",\
// "9\t2710\t96",\
// "4\t2711\t18",\
// "8\t2717\t5",\
// "0\t2726\t83",\
// "3\t2729\t97",\
// "6\t2733\t43",\
// "3\t2742\t8",\
// "7\t2748\t5",\
// "3\t2754\t17",\
// "0\t2755\t69",\
// "0\t2761\t94",\
// "1\t2769\t90",\
// "7\t2772\t58",\
// "3\t2780\t82",\
// "7\t2783\t29",\
// "8\t2784\t31",\
// "9\t2792\t22",\
// "7\t2793\t83",\
// "0\t2795\t79",\
// "9\t2804\t45",\
// "1\t2804\t65",\
// "1\t2811\t75",\
// "3\t2815\t35",\
// "6\t2823\t3",\
// "1\t2823\t94",\
// "1\t2831\t66",\
// "6\t2835\t96",\
// "9\t2839\t54",\
// "4\t2840\t62",\
// "7\t2843\t53",\
// "1\t2848\t49",\
// "9\t2854\t83",\
// "9\t2858\t53",\
// "0\t2865\t24",\
// "9\t2865\t9",\
// "3\t2872\t6",\
// "4\t2873\t25",\
// "5\t2882\t72",\
// "5\t2891\t36",\
// "1\t2899\t44",\
// "1\t2901\t27",\
// "7\t2901\t75",\
// "8\t2906\t18",\
// "4\t2913\t59",\
// "5\t2917\t47",\
// "5\t2919\t24",\
// "9\t2920\t79",\
// "8\t2921\t60",\
// "5\t2921\t22",\
// "1\t2921\t29",\
// "3\t2921\t93",\
// "0\t2921\t14",\
// "2\t2928\t75",\
// "0\t2932\t18",\
// "1\t2937\t44",\
// "9\t2944\t7",\
// "1\t2948\t65",\
// "8\t2950\t48",\
// "6\t2953\t62",\
// "0\t2955\t47",\
// "8\t2956\t98",\
// "5\t2956\t94",\
// "4\t2964\t67",\
// "7\t2966\t93",\
// "2\t2970\t25",\
// "7\t2974\t7",\
// "4\t2983\t20",\
// "0\t2983\t13",\
// "6\t2992\t2",\
// "2\t2994\t48",\
// "1\t3003\t59",\
// "1\t3010\t72",\
// "3\t3012\t35",\
// "2\t3017\t73",\
// "6\t3021\t87",\
// "6\t3025\t27",\
// "0\t3034\t96",\
// "8\t3035\t71",\
// "2\t3043\t79",\
// "9\t3043\t56",\
// "1\t3045\t67",\
// "7\t3052\t86",\
// "2\t3056\t5",\
// "2\t3059\t0",\
// "3\t3059\t16",\
// "5\t3066\t19",\
// "0\t3066\t56",\
// "9\t3075\t66",\
// "9\t3078\t68",\
// "9\t3080\t94",\
// "6\t3080\t30",\
// "1\t3082\t10",\
// "4\t3088\t74",\
// "2\t3088\t54",\
// "8\t3093\t25",\
// "7\t3101\t71",\
// "8\t3109\t48",\
// "9\t3118\t42",\
// "3\t3121\t83",\
// "5\t3127\t83",\
// "8\t3129\t34",\
// "5\t3135\t61",\
// "5\t3142\t76",\
// "6\t3145\t29",\
// "6\t3154\t38",\
// "3\t3161\t73",\
// "8\t3167\t30",\
// "1\t3173\t42",\
// "4\t3176\t84",\
// "4\t3178\t60",\
// "5\t3180\t47",\
// "5\t3180\t58",\
// "9\t3187\t24",\
// "8\t3193\t0",\
// "2\t3193\t38",\
// "8\t3200\t80",\
// "0\t3205\t29",\
// "3\t3205\t94",\
// "6\t3206\t99",\
// "7\t3214\t19",\
// "1\t3217\t0",\
// "6\t3225\t19",\
// "1\t3232\t1",\
// "2\t3234\t95",\
// "1\t3243\t16",\
// "5\t3248\t19",\
// "7\t3253\t69",\
// "8\t3259\t95",\
// "6\t3267\t19",\
// "5\t3275\t37",\
// "0\t3280\t9",\
// "5\t3282\t14",\
// "4\t3287\t86",\
// "5\t3293\t43",\
// "4\t3293\t80",\
// "6\t3298\t15",\
// "7\t3306\t85",\
// "6\t3315\t39",\
// "3\t3317\t88",\
// "6\t3323\t52",\
// "2\t3329\t5",\
// "3\t3333\t90",\
// "6\t3340\t81",\
// "8\t3347\t72",\
// "1\t3353\t83",\
// "5\t3358\t22",\
// "5\t3367\t89",\
// "9\t3369\t92",\
// "8\t3373\t58",\
// "8\t3382\t88",\
// "0\t3387\t96",\
// "8\t3387\t85",\
// "6\t3389\t59",\
// "8\t3398\t5",\
// "0\t3402\t71",\
// "0\t3410\t43",\
// "1\t3412\t10",\
// "0\t3414\t71",\
// "9\t3417\t42",\
// "8\t3420\t5",\
// "3\t3425\t17",\
// "5\t3429\t5",\
// "6\t3430\t88",\
// "1\t3439\t42",\
// "8\t3443\t80",\
// "3\t3452\t90",\
// "2\t3452\t6",\
// "2\t3454\t50",\
// "7\t3460\t18",\
// "1\t3469\t5",\
// "1\t3477\t25",\
// "7\t3477\t42",\
// "5\t3481\t66",\
// "6\t3489\t10",\
// "8\t3496\t53",\
// "2\t3500\t90",\
// "1\t3506\t44",\
// "3\t3515\t64",\
// "5\t3522\t84",\
// "7\t3524\t13",\
// "8\t3528\t2",\
// "3\t3531\t86",\
// "0\t3533\t61",\
// "1\t3539\t30",\
// "2\t3541\t23",\
// "0\t3546\t40",\
// "1\t3552\t76",\
// "9\t3558\t72",\
// "3\t3559\t0",\
// "8\t3561\t40",\
// "9\t3561\t62",\
// "8\t3567\t48",\
// "0\t3568\t1",\
// "5\t3572\t68",\
// "7\t3581\t12",\
// "7\t3584\t66",\
// "8\t3584\t82",\
// "9\t3589\t25",\
// "3\t3589\t93",\
// "0\t3597\t4",\
// "7\t3603\t96",\
// "6\t3612\t96",\
// "2\t3618\t29",\
// "9\t3624\t11",\
// "5\t3633\t48",\
// "3\t3641\t55",\
// "5\t3646\t97",\
// "0\t3655\t11",\
// "0\t3663\t57",\
// "5\t3668\t23",\
// "2\t3668\t48",\
// "5\t3674\t70",\
// "1\t3676\t75",\
// "0\t3679\t5",\
// "8\t3682\t36",\
// "9\t3690\t27",\
// "7\t3699\t53",\
// "5\t3702\t96",\
// "4\t3708\t82",\
// "4\t3714\t77",\
// "3\t3715\t14",\
// "1\t3715\t97",\
// "2\t3724\t88",\
// "1\t3726\t33",\
// "5\t3728\t1",\
// "1\t3737\t27",\
// "6\t3739\t54",\
// "6\t3746\t60",\
// "9\t3753\t39",\
// "7\t3759\t95",\
// "7\t3760\t97",\
// "7\t3762\t11",\
// "5\t3762\t88",\
// "9\t3763\t96",\
// "6\t3771\t97",\
// "6\t3771\t42",\
// "0\t3779\t5",\
// "3\t3780\t32",\
// "7\t3786\t48",\
// "6\t3786\t41",\
// "5\t3791\t80",\
// "8\t3799\t65",\
// "9\t3800\t48",\
// "5\t3804\t93",\
// "8\t3805\t56",\
// "2\t3810\t85",\
// "8\t3819\t39",\
// "6\t3825\t55",\
// "9\t3832\t44",\
// "1\t3835\t70",\
// "8\t3840\t33",\
// "7\t3842\t8",\
// "0\t3848\t37",\
// "4\t3856\t73",\
// "0\t3865\t49",\
// "2\t3870\t80",\
// "3\t3872\t24",\
// "0\t3878\t36",\
// "2\t3880\t2",\
// "5\t3887\t19",\
// "7\t3891\t32",\
// "7\t3895\t30",\
// "1\t3896\t91",\
// "7\t3900\t46",\
// "8\t3904\t94",\
// "7\t3906\t26",\
// "4\t3911\t80",\
// "8\t3920\t9",\
// "2\t3924\t88",\
// "7\t3931\t83",\
// "7\t3932\t77",\
// "7\t3936\t29",\
// "6\t3942\t72",\
// "2\t3946\t51",\
// "0\t3954\t81",\
// "5\t3957\t76",\
// "0\t3964\t40",\
// "0\t3967\t26",\
// "8\t3969\t27",\
// "7\t3972\t55",\
// "3\t3981\t53",\
// "1\t3987\t8",\
// "7\t3991\t31",\
// "1\t3998\t2",\
// "6\t3999\t35",\
// "7\t4007\t34",\
// "8\t4013\t14",\
// "7\t4018\t68",\
// "9\t4018\t97",\
// "9\t4023\t50",\
// "0\t4023\t52",\
// "7\t4032\t16",\
// "1\t4038\t50",\
// "0\t4040\t77",\
// "1\t4047\t64",\
// "3\t4054\t53",\
// "7\t4061\t39",\
// "4\t4069\t81",\
// "8\t4069\t22",\
// "6\t4072\t22",\
// "8\t4077\t5",\
// "9\t4078\t5",\
// "5\t4078\t49",\
// "9\t4081\t15",\
// "2\t4082\t53",\
// "5\t4089\t28",\
// "8\t4097\t82",\
// "7\t4102\t99",\
// "4\t4111\t39",\
// "8\t4111\t91",\
// "7\t4116\t39",\
// "0\t4117\t41",\
// "6\t4126\t30",\
// "0\t4129\t13",\
// "0\t4133\t67",\
// "1\t4133\t38",\
// "6\t4138\t48",\
// "8\t4144\t10",\
// "3\t4146\t39",\
// "8\t4153\t6",\
// "6\t4161\t15",\
// "7\t4162\t14",\
// "4\t4162\t60",\
// "8\t4165\t69",\
// "6\t4173\t32",\
// "5\t4176\t98",\
// "6\t4185\t54",\
// "8\t4185\t19",\
// "9\t4189\t49",\
// "4\t4198\t14",\
// "5\t4198\t85",\
// "8\t4204\t32",\
// "3\t4207\t57",\
// "5\t4208\t18",\
// "0\t4216\t88",\
// "0\t4221\t11",\
// "7\t4230\t80",\
// "5\t4237\t62",\
// "8\t4246\t25",\
// "5\t4250\t42",\
// "3\t4251\t45",\
// "1\t4254\t36",\
// "9\t4259\t89",\
// "3\t4264\t91",\
// "0\t4267\t14",\
// "4\t4272\t38",\
// "1\t4273\t4",\
// "9\t4275\t25",\
// "8\t4282\t32",\
// "4\t4288\t24",\
// "9\t4290\t79",\
// "3\t4299\t95",\
// "7\t4301\t62",\
// "9\t4308\t13",\
// "1\t4316\t83",\
// "0\t4317\t62",\
// "7\t4324\t49",\
// "3\t4329\t29",\
// "6\t4334\t51",\
// "8\t4335\t68",\
// "6\t4335\t45",\
// "5\t4337\t43",\
// "5\t4341\t22",\
// "4\t4344\t15",\
// "0\t4351\t44",\
// "2\t4354\t18",\
// "9\t4355\t14",\
// "7\t4355\t26",\
// "6\t4355\t36",\
// "4\t4357\t8",\
// "5\t4365\t56",\
// "1\t4370\t62",\
// "4\t4371\t61",\
// "6\t4373\t23",\
// "2\t4374\t92",\
// "2\t4382\t8",\
// "8\t4385\t39",\
// "0\t0\t0",\
// "1\t0\t0",\
// "2\t0\t0",\
// "3\t0\t0",\
// "4\t0\t0",\
// "5\t0\t0",\
// "6\t0\t0",\
// "7\t0\t0",\
// "8\t0\t0",\
// "9\t0\t0"};