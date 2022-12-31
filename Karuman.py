import math
import matplotlib.patches as patches
import numpy as np
from collections import namedtuple
# 円をプロットする関数
# radiusで半径、nで角度360°の分割数を指定
# center_markerにTrueを指定したときは円の中心点を表示する
def draw_circle(ax, radius, center=(0,0), n=129,
                color="blue", center_marker=True):

    # 角度θの刻みデータ
    theta = np.linspace(0, 2*np.pi, n)

    # 円周上のx座標とy座標
    x = center[0] + radius * np.cos(theta)
    y = center[1] + radius * np.sin(theta)

    # x軸とy軸のラベルを表示
    ax.set_xlabel("x", fontsize = 16)
    ax.set_ylabel("y", fontsize = 16)

    # データをプロット
    ax.plot(x, y, color=color)
    
    # 原点Oをプロット
    ax.scatter(0, 0, color="black")

    # 円の中心座標をプロット
    if center_marker == True:
        ax.scatter(center[0], center[1], color=color)

def pointer(axes, x, y, text, angle = 45,
            textsize = 12, textcolor = "black", pad = 0.2,
            psize = None, pcolor = None, marker = None,
            cmap = None, norm = None, alpha = None,
            linewidths = None, edgecolors = None):
    # 点をプロット
    axes.scatter(x, y, s = psize, c = pcolor,
                 marker = marker, cmap = cmap, norm = norm,
                 alpha = alpha, linewidths = linewidths,
                 edgecolors = edgecolors)
    
    # 数学関数モジュールをインポート
    import math
    
    # テキストの配置角度をラジアンに変換
    text_angle = angle * math.pi / 180
    
    # テキストの位置を計算
    loc_x = x + pad * math.cos(text_angle)
    loc_y = y + pad * math.sin(text_angle)
    
    # テキストを配置
    axes.text(loc_x, loc_y, text,
              fontsize = textsize, color = textcolor)
  
def gaussian_multiply(g1, g2):
    mean = (g1.var * g2.mean + g2.var * g1.mean) / (g1.var + g2.var)
    variance = (g1.var * g2.var) / (g1.var + g2.var)
    return gaussian(mean, variance)

def update(prior, likelihood):
    posterior = gaussian_multiply(likelihood, prior)
    return posterior

def observeLidarPoint(lidarPoint, std):
    return lidarPoint + (randn() * std)
def predict(pos, movement):
    return gaussian(pos.mean + movement.mean, pos.var + movement.var)
    
gaussian = namedtuple('Gaussian', ['mean', 'var'])
temp_change = 0
lidarPoint_std = .05
process_var = .02**2 #移動量の分散
actual_lidarPointX = 3.15
actual_lidarPointY = -2.18

x = gaussian(25., 1000.) # 初期状態
y = gaussian(25., 1000.) # 初期状態
process_model = gaussian(0., process_var)

N = 50
zsX = [
3.17636,
3.15423,3.18159,
3.1567,
3.15265,
3.16303,
3.15947,
3.16303,
3.1567,
3.16303,
3.15947,
3.16303,
3.1567,
3.15265,
3.16303,
3.15947,
3.16303,
3.15947,
3.1567,]
zsY =[
-2.21138,
 -2.18025,
 -2.17731,
	-2.1528,
	-2.1404,
	-2.1312,
 -2.19897,
 	-2.1528,
	-2.1404,
	-2.1312,
 -2.19897,
 -2.18646,
 -2.16606,
 -2.18646,
 -2.16606,
	-2.1462,]
psX = []
psY = []
estimatesX = []
estimatesY = []

prior = predict(x, process_model)
x = update(prior, gaussian(zsX[0], lidarPoint_std**2))
prior = predict(y, process_model)
y = update(prior, gaussian(zsY[0], lidarPoint_std**2))

for z in zsX:
    prior = predict(x, process_model)
    x = update(prior, gaussian(z, lidarPoint_std**2))

    # グラフにするために記録する
    estimatesX.append(x.mean)
    psX.append(x.var)

for z in zsY:
    prior = predict(y, process_model)
    y = update(prior, gaussian(z, lidarPoint_std**2))

    # グラフにするために記録する
    estimatesY.append(y.mean)
    psY.append(y.var)

# フィルタの出力と分散をプロットする
ax = plt.axes()
ax.plot(zsX,marker='.',label='observed X')
ax.plot(estimatesX, marker='.', label='estimated X')
plt.xlabel('point index')
plt.ylabel('x coordinate [m]')
plt.legend()
plt.show()
plt.plot(zsY, marker='.',label='observed Y')
plt.plot(estimatesY, marker='.', label='estimated Y')
plt.xlabel('point index')
plt.ylabel('y coordinate [m]')
plt.show()
theta = math.atan2(-2.2, 3.2)
point = [[estimatesX[-1]+0.05*math.cos(theta), estimatesY[-1]+0.05*math.sin(theta)],
         [3.2, -2.2]]

fig = plt.figure(figsize=(6, 6))
ax = fig.add_subplot(111)
ax.grid()
ax.set_title("Pole Circle", fontsize = 16)
ax.set_xlim(3.1, 3.3)
ax.set_ylim(-2.3, -2.1)
plt.xlabel('x coordinate [m]')
plt.ylabel('y coordinate [m]')
ax.text(3.185, -2.175, "Err=0.7943670369[cm]", size=15)

draw_circle(ax, radius=0.05, center=(point[0][0], point[0][1]))
draw_circle(ax, radius=0.05,color="red", center=(point[1][0], point[1][1]))
# 原点と単位円を結ぶ直線をプロット
ax.plot([point[0][0],point[1][0]],[point[0][1],point[1][1]], color="black")
a=np.array(point[0])
b=np.array(point[1])
print("Error=\t",100*np.linalg.norm(b-a), "cm")
