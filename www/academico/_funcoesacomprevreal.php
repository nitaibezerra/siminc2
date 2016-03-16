<?php
function barraDeProgresso($valor1 = 100, $valor2 = 10, $cor = null){
   
	//$valor1 = 100;
    //$valor2 = 12;
	
    if($valor1 <= 0){
        $valor1 = 1;
        $valor2 = 0;
    }
   
    $percent = ($valor2 / $valor1) * 100;
   
    //regras de cor
    if($percent == 0){
        $corDefault = "#000000";
        $corFonte = "#000000";
    }elseif($percent >= 0 && $percent <= 49){
        $corDefault = "#FF0000";
        $corFonte = "#000000";
    }elseif($percent > 49 && $percent <= 74){
        $corDefault = "#FFFF00";
        $corFonte = "#000000";
    }elseif($percent > 74 && $percent <= 99){
        $corDefault = "#00EE00";
        $corFonte = "#FFFFFF";
    }elseif($percent == 100){
        $corDefault = "#1E90FF";
        $corFonte = "#FFFFFF";
    }else{
        $corDefault = "#1E90FF";
        $corFonte = "#FFFFFF";
        $texto = round($percent,2);
        $percent = 100;
    }
    
    $percent = round($percent,2);
   
    $cor = !$cor ? $corDefault : $cor;
    
    if($percent) {
	    $html .= "<div style=\"width:50px;border:1px solid #ccc;height:15px;text-align:left;background-color:#FFFFFF\" >";
	    $html .= "<div style=\"position:relative;z-index:9;width:$percent%;background:$cor;height:12px;color:$corFonte;text-align:right;padding:3px 0px 0px 0px;\" >";
	    $html .= "</div>";
	    $html .= "</div>";
	    $html .= "<div style=\"color:$corFonte;width:50px;text-align:center;position:relative;z-index:10;margin-top:-16px\" >".(!$texto ? $percent : $texto)."%</div>";
    } else {
    	$html .= "-";	
    }
    
    return $html;
   
}

function mascaraglobal($value, $mask) {
	
	return formata_valor( $value , 0);
}
?>