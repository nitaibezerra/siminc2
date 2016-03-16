<?php
class MedidorDesempenho{
	
	public function __construct($arrDados,$valor,$meta = null)
	{
		$tamanho = 1.6;
		$altura = '10';
		$porcentagem_final = $valor;
		if($valor > 100){
			$valor = 100;
		}
		$valor = $valor-50;
		$valor = str_replace(".",",",$valor);
		$porcentagem_final = str_replace(".",",",$porcentagem_final);
		if(strstr($_SERVER['HTTP_USER_AGENT'],"Firefox")){//Firefox
			$total = $arrDados[count($arrDados)-1]['fim'];
			echo "<div style=\"width:".($tamanho*$total)."px;height:50px;\" >";
			echo "<div style=\"position:relative;z-index:1;clear:both;float:left;width:".($tamanho*$total)."px;height:{$altura}px;border:0px\" >";
			echo "<div style=\"float:left;width:0px;height:{$altura}px;border:0px;text-align:right\" >";
			echo "<span style=\"margin-left:-2px\" ><b></b></span>";
			echo "</div>";
			foreach($arrDados as $n => $dado){
				$inicio = $dado['inicio'];
				$fim 	= $dado['fim'];
				$cor 	= $dado['cor'];
				$paramCor[$n] = Array('inicio'=>$inicio,'fim'=>$fim,'cor'=>$cor);
				$cor_antiga = $cor;
				echo "<div style=\"float:left;width:".((($fim - $inicio)*$tamanho)*2)."px;height:{$altura}px;border:0px;text-align:right\" >";
				echo "<span style=\"margin-left:2px\" ><b>".($n == count($arrDados) -1 ? $meta : "")."</b></span>";
				echo "</div>";
			}
			echo "</div>";
			echo "<div style=\"".($porcentagem_final <= $arrDados[0]['inicio'] ? "left:-11;" : "")."margin-top:0px;clear;both;position:relative;z-index:2;float:left;width:".($valor > 0 ? (($tamanho*$valor*2)+($valor != $arrDados[0]['inicio'] ? 15 : 15)) : 1)."px;height:1px;border:0px;text-align:right\" >";
			$tam = strlen($valor);
			for($i=0;$i<(4-$tam);$i++){
				$espaco.= "";
			}
			if( $porcentagem_final < $paramCor[0]['fim'] ){
				$corSeta = 'vermelho';
			}elseif( $porcentagem_final < $paramCor[1]['fim'] ){
				$corSeta = 'amarelo';
			}else{
				$corSeta = 'verde';
			}
			
			echo "<img width=27 style=\"margin-top:15px\" height=20 src=\"../imagens/trinangulo_$corSeta.png\" /><br /><b>$porcentagem_final%</b>$espaco";
			echo "</div>";
			echo "<div style=\"margin-top:12px;position:relative;z-index:1;clear:both;float:left;width:".($tamanho*$total)."px;height:{$altura}px;border:solid 1px\" >";
			foreach($arrDados as $n => $dado){
				$inicio = $dado['inicio'];
				$fim 	= $dado['fim'];
				$cor 	= $dado['cor'];
				$cor_antiga = $arrDados[$n - 1]['cor'] ? $arrDados[$n - 1]['cor'] : $cor;
				echo "<div style=\"float:left;width:".((($fim - $inicio)*$tamanho)*2)."px;height:{$altura}px;border:0px;background-color:$cor;\" >";
				echo "</div>";
			}
			echo "</div>";
			echo "</div>";
		}elseif(strstr($_SERVER['HTTP_USER_AGENT'],"MSIE")){//IE
			$total = $arrDados[count($arrDados)-1]['fim'];
			echo "<div style=\"margin:0 auto;text-align:left;width:".($tamanho*$total)."px;height:50px\" >";
			echo "<div style=\"position:relative;z-index:1;clear:both;float:left;width:".($tamanho*$total)."px;height:{$altura}px;border:0px\" >";
			echo "<div style=\"float:left;border:0px;text-align:right\" >";
			echo "<span style=\"\" ><b>{$arrDados[0]['inicio']}</b></span>";
			echo "</div>";
			foreach($arrDados as $n => $dado){
				$inicio = $dado['inicio'];
				$fim 	= $dado['fim'];
				$cor 	= $dado['cor'];
				$paramCor[$n] = Array('inicio'=>$inicio,'fim'=>$fim,'cor'=>$cor);
				$cor_antiga = $arrDados[$n - 1]['cor'] ? $arrDados[$n - 1]['cor'] : $cor;
				echo "<div style=\"float:left;width:".((($fim - $inicio)*$tamanho)-2)."px;height:{$altura}px;border:solid 0px;text-align:right\" >";
				echo "<span style=\"\" ><b>$fim</b></span>";
				echo "</div>";
			}
			echo "</div>";
			$tam = strlen($valor);
			for($i=0;$i<(4-$tam);$i++){
				$espaco.= "&nbsp";
			}
			if( $porcentagem_final < $paramCor[0]['fim'] ){
				$corSeta = 'vermelho';
			}elseif( $porcentagem_final < $paramCor[1]['fim'] ){
				$corSeta = 'amarelo';
			}else{
				$corSeta = 'verde';
			}
			echo "<div style=\"margin-top:12px;clear;both;position:relative;z-index:2;float:left;width:".((($tamanho*$valor)+($valor != $arrDados[0]['inicio'] ? 18 : 1))-4)."px;height:1px;border:0px;text-align:right\" >";
			echo "<img ".($valor == $arrDados[0]['inicio'] ? "style=\"position:relative;z-index:0;margin-left:-28\"" : "")." src=\"../imagens/seta_$corSeta.png\" /><br />".($valor == $arrDados[0]['inicio'] ? "<br/>" : "")."<b>$porcentagem_final</b>$espaco";
			echo "</div>";
			echo "<div style=\"margin-top:12px;position:relative;z-index:1;clear:both;float:left;width:".($tamanho*$total)."px;height:{$altura}px;border:solid 1px;background-color:".$arrDados[count($arrDados)-1]['cor']."\" >";
			foreach($arrDados as $n => $dado){
				$inicio = $dado['inicio'];
				$fim 	= $dado['fim'];
				$cor 	= $dado['cor'];
				$cor_antiga = $arrDados[$n - 1]['cor'] ? $arrDados[$n - 1]['cor'] : $cor;
				echo "<div style=\"float:left;width:".((($fim - $inicio)*$tamanho)-0.1)."px;height:{$altura}px;border:0px; background-color:$cor;\" >";
				echo "</div>";
			}
			echo "</div>";
			echo "</div>";
		}else{//Chrome
			$total = $arrDados[count($arrDados)-1]['fim'];
			echo "<div style=\"width:".($tamanho*$total)."px;height:50px;\" >";
			echo "<div style=\"position:relative;z-index:1;clear:both;float:left;width:".($tamanho*$total)."px;height:{$altura}px;border:0px\" >";
			echo "<div style=\"float:left;width:0px;height:{$altura}px;border:0px;text-align:right\" >";
			echo "<span style=\"margin-left:-2px\" ><b>{$arrDados[0]['inicio']}</b></span>";
			echo "</div>";
			foreach($arrDados as $n => $dado){
				$inicio = $dado['inicio'];
				$fim 	= $dado['fim'];
				$cor 	= $dado['cor'];
				$paramCor[$n] = Array('inicio'=>$inicio,'fim'=>$fim,'cor'=>$cor);
				$cor_antiga = $arrDados[$n - 1]['cor'] ? $arrDados[$n - 1]['cor'] : $cor;
				echo "<div style=\"float:left;width:".(($fim - $inicio)*$tamanho)."px;height:{$altura}px;border:0px;text-align:right\" >";
				echo "<span style=\"margin-right:-7px\" ><b>$fim</b></span>";
				echo "</div>";
			}
			if( $porcentagem_final < $paramCor[0]['fim'] ){
				$corSeta = 'vermelho';
			}elseif( $porcentagem_final < $paramCor[1]['fim'] ){
				$corSeta = 'amarelo';
			}else{
				$corSeta = 'verde';
			}
			echo "</div>";
			echo "<div style=\"margin-top:12px;clear;both;position:relative;z-index:2;float:left;width:".(($tamanho*$valor)+($valor != $arrDados[0]['inicio'] ? 15 : 1))."px;height:1px;border:0px;text-align:right\" >";
			$tam = strlen($valor);
			for($i=0;$i<(4-$tam);$i++){
				$espaco.= "&nbsp";
			}
			
			echo "<img ".($valor == $arrDados[0]['inicio'] ? "style=\"margin-left:-14\"" : "")." src=\"../imagens/seta_$corSeta.png\" /><br /><b>$porcentagem_final</b>$espaco";
			echo "</div>";
			echo "<div style=\"margin-top:12px;position:relative;z-index:1;clear:both;float:left;width:".($tamanho*$total)."px;height:{$altura}px;border:solid 1px\" >";
			foreach($arrDados as $n => $dado){
				$inicio = $dado['inicio'];
				$fim 	= $dado['fim'];
				$cor 	= $dado['cor'];
				$cor_antiga = $arrDados[$n - 1]['cor'] ? $arrDados[$n - 1]['cor'] : $cor;
				//echo "<div style=\"float:left;width:".(($fim - $inicio)*$tamanho)."px;height:{$altura}px;border:0px;background:-moz-linear-gradient(left, $cor_antiga, $cor);\" >";
				echo "<div style=\"float:left;width:".(($fim - $inicio)*$tamanho)."px;height:{$altura}px;border:0px;background-color:$cor;\" >";
				echo "</div>";
			}
			echo "</div>";
			echo "</div>";	
		}
		
	}
	
}