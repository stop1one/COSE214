#! /usr/bin/env Rscript
png("convex.png", width=700, height=700)
plot(1:10000, 1:10000, type="n")

#points
points(194,6093)
points(278,338)
points(505,4238)
points(506,1783)
points(631,2248)
points(733,5105)
points(755,1608)
points(988,3309)
points(1021,927)
points(1747,3059)
points(1837,1142)
points(1869,2237)
points(1968,7762)
points(2161,406)
points(2183,4265)
points(2207,3166)
points(2337,3311)
points(2350,2210)
points(2661,902)
points(2701,6832)
points(2737,864)
points(2874,5515)
points(2930,2396)
points(2946,9489)
points(3074,1753)
points(3080,9314)
points(3125,5633)
points(3143,6312)
points(3151,3041)
points(3162,5538)
points(3325,6132)
points(3382,4004)
points(3405,3604)
points(3448,1012)
points(3472,8703)
points(3695,3587)
points(3787,5625)
points(3896,3021)
points(3944,1355)
points(3982,6509)
points(4008,7850)
points(4051,6025)
points(4062,9367)
points(4195,382)
points(4237,5467)
points(4306,7713)
points(4416,3359)
points(4750,5998)
points(4852,8650)
points(4869,2704)
points(4990,5166)
points(5011,5780)
points(5090,4325)
points(5146,8760)
points(5167,7415)
points(5197,5869)
points(5240,2560)
points(5386,805)
points(5404,5303)
points(5404,8591)
points(5428,6358)
points(5637,7758)
points(5703,4589)
points(5724,7697)
points(5758,620)
points(5909,5363)
points(5972,8684)
points(6008,971)
points(6036,4717)
points(6170,2761)
points(6342,2490)
points(6396,7831)
points(6658,7966)
points(6843,7004)
points(7194,3851)
points(7250,2110)
points(7261,3321)
points(7270,6585)
points(7386,5865)
points(7674,1564)
points(7753,8627)
points(7758,8685)
points(7790,9227)
points(7840,8658)
points(7990,3549)
points(7994,3691)
points(8135,5925)
points(8253,8679)
points(8272,4974)
points(8345,9547)
points(8425,6377)
points(8609,1690)
points(8713,1055)
points(8803,7918)
points(9223,5791)
points(9526,4372)
points(9566,3040)
points(9594,8103)
points(9860,5766)
points(9988,7364)

#line segments
segments(194,6093,2946,9489)
segments(2946,9489,8345,9547)
segments(8345,9547,9594,8103)
segments(9594,8103,9988,7364)
segments(9988,7364,9860,5766)
segments(9860,5766,9566,3040)
segments(9566,3040,8713,1055)
segments(8713,1055,4195,382)
segments(4195,382,278,338)
segments(278,338,194,6093)
dev.off()
