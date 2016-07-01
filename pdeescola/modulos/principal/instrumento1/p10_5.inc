<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

function salva() {
	global $db;
	
    for( $y = 0; $y < count( $_POST["serie_id"]); $y++ )
    {
        $id_turno = ($y + 1);
    
        if($_POST["submetido"]) 
        {
            $pdeid         = $_POST["pdeid"];
            $serieId       = $_POST["serie_id"];
            $matfinal      = $_POST["matfinal"];
            $qtd7anos      = $_POST["qtd7anos"];
            $qtd8anos      = $_POST["qtd8anos"];
            $qtd9anos      = $_POST["qtd9anos"];
            $qtd10anos     = $_POST["qtd10anos"];
            $qtd11anos     = $_POST["qtd11anos"];
            $qtd12anos     = $_POST["qtd12anos"];
            $qtdmais12anos = $_POST["qtdmais12anos"];
           
     
   
                for($i=1; $i <= count($serieId[$y]); $i++) 
                {
                     //;
                     $cont = $i + 1;
     
                    //echo "y = $y | i = $i <BR>{$qtd8anos[$y][$i]}<BR>";
                    $matfinal[$y][$i] = ($matfinal[$y][$i] == NULL) ? 'NULL' : $matfinal[$y][$i];
					$qtd7anos[$y][$i] = ($qtd7anos[$y][$i] == NULL) ? 'NULL' : $qtd7anos[$y][$i]; 
                    $qtd8anos[$y][$i] = ($qtd8anos[$y][$i] == NULL) ? 'NULL' : $qtd8anos[$y][$i];
                    $qtd9anos[$y][$i] = ($qtd9anos[$y][$i] == NULL) ? 'NULL' : $qtd9anos[$y][$i];
                    $qtd10anos[$y][$i] = ($qtd10anos[$y][$i] == NULL) ? 'NULL' : $qtd10anos[$y][$i];
                    $qtd11anos[$y][$i] = ($qtd11anos[$y][$i] == NULL) ? 'NULL' : $qtd11anos[$y][$i];
                    $qtd12anos[$y][$i] = ($qtd12anos[$y][$i] == NULL) ? 'NULL' : $qtd12anos[$y][$i];
                    $qtdmais12anos[$y][$i] = ($qtdmais12anos[$y][$i] == NULL) ? 'NULL' : $qtdmais12anos[$y][$i];
                    
                     for($k=1; $k<=7; $k++) 
                     {
                         /*
                         echo "y = $y | i = $i <=> {$serieId[$y][$i]}";
                         if (!$serieId[$y][$i])
                             die();
                        */
                     
    /*
     * simplesmente para efeito debug
     */
                     	/*
    $sql = "SELECT disid FROM pdeescola.distorcaoidadeserie
                                               WHERE pdeid = ".$pdeid."
                                                 AND sceid = ".$serieId[$y][$i]."
                                                 AND disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA."
                                                 AND faeid = ".$k."
                                                 AND titid = ".$id_turno."";
*/
                        
                        $existe = $db->pegaUm("SELECT disid FROM pdeescola.distorcaoidadeserie
                                               WHERE pdeid = ".$pdeid."
                                                 AND sceid = ".$serieId[$y][$i]."
                                                 AND disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA."
                                                 AND faeid = ".$k."
                                                 AND titid = ".$id_turno);
                
                        if($k == 1) { $qtdAlunos = $qtd7anos[$y][$i]; } 
                        if($k == 2) { $qtdAlunos = $qtd8anos[$y][$i]; } 
                        if($k == 3) { $qtdAlunos = $qtd9anos[$y][$i]; }
                        if($k == 4) { $qtdAlunos = $qtd10anos[$y][$i]; }
                        if($k == 5) { $qtdAlunos = $qtd11anos[$y][$i]; }
                        if($k == 6) { $qtdAlunos = $qtd12anos[$y][$i]; }
                        if($k == 7) { $qtdAlunos = $qtdmais12anos[$y][$i]; }		
                
                        /*
                        echo ("k = $k | y = $y | i = $i");
                       
                        echo("qtd alunos");
                        echo("<pre>");
                        print_r( $qtd8anos );
                        echo("</pre>");
                        //echo("<script>alert('passou $k');\n</script>");
                        */
                         
                        if($existe == "") 
                        {
                                        $db->executar("
                                            INSERT INTO pdeescola.distorcaoidadeserie
                                            (
                                             pdeid,
                                             sceid,
                                             disqtdmatriculafinal,
                                             disqtdalunos,
                                             faeid,
                                             disanoreferencia,
                                             disdataatualizacao, 
                                             titid
                                             ) 
                                           VALUES
                                           (
                                           '$pdeid',
                                           '{$serieId[$y][$i]}',
                                           {$matfinal[$y][$i]},
                                           $qtdAlunos,
                                           '$k',
                                           ".ANO_EXERCICIO_PDE_ESCOLA.",
                                           now(), 
                                           $id_turno
                                           )
                                           ");
                        } 
                        else 
                        {
                            $db->executar("UPDATE 
                                               pdeescola.distorcaoidadeserie
                                           SET 
                                               disqtdmatriculafinal = ".$matfinal[$y][$i].",
                                               disqtdalunos = ".$qtdAlunos."
                                           WHERE 
                                                disid = ".$existe);
                        }
                                
                     }
                       
                }
 
            
            $epfid = $db->pegaUm("SELECT
									epfid
								  FROM
								  	pdeescola.estruturaperfilfuncionamento
								  WHERE
								  	trim(epfnomearquivo) = 'p10_5' AND
								  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
            
            $existe = $db->pegaUm("SELECT
                                    pepid
                                 FROM
                                    pdeescola.pdeepf
                                 WHERE
                                    pdeid = ".$pdeid." AND
                                    epfid = ".$epfid);
            
            if($existe == NULL) {
                $sql = "INSERT INTO pdeescola.pdeepf(pdeid,epfid) VALUES(".$pdeid.",".$epfid.")";
                $db->executar($sql);
            }
 
	        $existe_preenchimento = $db->pegaUm("SELECT 
													pprid
												 FROM 
												 	pdeescola.pdepreenchimento
												 WHERE
												 	pdeid = '$pdeid'
												 AND
												 	ppritem = 'p10_5'");
			if( $existe_preenchimento == NULL )
			{
				$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p10_5', '{$_REQUEST['anoreferencia']}')";
				$db->executar( $sql_p );
 
			}
		$db->commit();  
        }
    }
 
}

$pdeid = $_SESSION["pdeid"];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

/***********************************************************
 * Componente que irá montar os butões do formulário
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serão carregados pelo 
 * 										   __construct "Padrões à classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   não precisa dar include do arquivo na página.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos métodos 
 * 																		1º parametro
 * 																		passa a função que será aplicada nos botões 
 * 																		"anterior, próximo, salvar anterior, salvar próximo e salvar".
 * 																		2º parametro
 * 																		será um array de "índice" igual ao parametro da funcão 
 * 																		passada no 1º parametro e o valor será o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulário "dentro da tabela ainda", este monta os butões.
 * 											  O parametro passado refere-se a função javascript que validará o formulário,
 * 											  devendo esta submeter o formulário. 
 * 
 ***********************************************************/

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1';
monta_titulo($titulo_modulo, '');
echo cabecalhoPDE();

?>

<script language="javascript" type="text/javascript">
	var series = new Array();
</script>
<?=subCabecalho('10.5 - Distorção idade-série - 1ª - 4ª série/ 1º - 5º ano do Ensino Fundamental (ano anterior)'); ?>
<form method="post" id="p96_form_aproveitamento" name="p96_form_aproveitamento">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela listagem" align="center">
 	<tr>
  	<td colspan="3">
  		
  		 <?php 
	  		 $anoex   = ANO_EXERCICIO_PDE_ESCOLA;
	  		 $anoex2  = $anoex +1;
    		 $anoex3  = $anoex2 +1;
//  		 $anoex4  = $anoex3 -1;
	  		 $arrUrl  = $_SERVER['argv'][0];
		  	 $arrUrl  = explode("&", $arrUrl);
			 $arrItem = explode("/", $arrUrl[0]);	 
			 $indice  = (count($arrItem) - 1);
		  	 $ppritem = $arrItem[$indice];
	  		 
	  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
	  		 					  FROM pdeescola.pdepreenchimento 
	  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
	  		 					  AND ppritem = '$ppritem'";
	  		 
			 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<!--   
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
			-->
		 </select>   
    </td>
  </tr>
</table>
<?php
$sql_turno = "SELECT titid, titdescricao FROM pdeescola.tipoturno ORDER BY titid";
$turno = $db->carregar ( $sql_turno );

for( $y = 0; $y < count( $turno ); $y++ )
{ 
    
    $titid = $turno[$y]['titid'];
?>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <thead>
    <td colspan="11">
    <div class="SubtituloEsquerda" style="padding: 5px; text-align: left; "><?php echo $turno[$y]['titdescricao']; ?></div>
    </td>
    </thead>
    <thead>
        <tr>
            <th width="5%">SÉRIE/ANO</th>
            <th width="10%">Matrícula Final (A)</th>
            <th width="10%">Até 7 anos</th>
            <th width="10%">Até 8 anos</th>
            <th width="10%">Até 9 anos</th>
            <th width="10%">Até 10 anos</th>
            <th width="10%">Até 11 anos</th>
            <th width="10%">Até 12 anos</th>
            <th width="10%">Mais de 12 anos</th>
            <th width="15%">Total de alunos com idade superior à série respectiva (B)</th>
            <th width="10%">Taxa de Distorção (B/A) x 100</th>
        </tr>
    </thead>
    <tbody>
        <?php
            $serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
                                    WHERE tmeid = 2 AND scetipo = 'S' AND scedescricao in ('0/1º','1ª/2º','2ª/3º','3ª/4º','4ª/5º')
                                    ORDER BY sceid");
            
            $totalMatFinal = 0;
			$total7anos = 0;
            $total8anos = 0;
            $total9anos = 0;
            $total10anos = 0;
            $total11anos = 0;
            $total12anos = 0;
            $totalMais12anos = 0;
            $totalIdadeSuperior = 0;
		
            
            for($i=0; $i<count($serie); $i++) {
                echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."');  </script>";
                $x++;
                $dados = $db->carregar("SELECT
                                            matfinal,	
                                            sum(qtdalunos0) as qtd7anos,
                                            sum(qtdalunos1) as qtd8anos,
                                            sum(qtdalunos2) as qtd9anos,
                                            sum(qtdalunos3) as qtd10anos,
                                            sum(qtdalunos4) as qtd11anos,
                                            sum(qtdalunos5) as qtd12anos,
                                            sum(qtdalunos6) as qtdmais12anos
                                        FROM
                                        (
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            dis.disqtdalunos as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 1 AND
                                            titid = '{$turno[$y]['titid']}'
                                            
                                        
                                        UNION ALL
                                        
                                        
                                        SELECT 
                                        	
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            dis.disqtdalunos as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 2 AND
                                            titid = '{$turno[$y]['titid']}'
                                            
                                        
                                        UNION ALL
                                        
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            dis.disqtdalunos as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 3 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            dis.disqtdalunos as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 4 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            dis.disqtdalunos as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 5 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            dis.disqtdalunos as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 6 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            dis.disqtdalunos as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 7 AND
                                            titid = '{$turno[$y]['titid']}'
                                        ) as foo 
                                        GROUP BY matfinal");
                
    
                        
                for($j=0; $j<count($dados); $j++) {
                    $idadeSuperior = ($dados[$j]["qtd7anos"] + $dados[$j]["qtd8anos"] + $dados[$j]["qtd9anos"] + $dados[$j]["qtd10anos"]
                                      + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);
                    $idadeSuperior = ($idadeSuperior == 0) ? '' : $idadeSuperior;
                                      
                    if($dados[$j]["matfinal"] != 0)
                    {
                        $taxa = str_replace('.',',',number_format((($idadeSuperior / $dados[$j]["matfinal"]) * 100),2));
                    	$taxaDistorcao = round( $taxa ).'%';
                    }
                    else
                    {
                        $taxaDistorcao = "";
                    }
                    $totalMatFinal += $dados[$j]["matfinal"];
                    $total7anos += $dados[$j]["qtd7anos"];
                    $total8anos += $dados[$j]["qtd8anos"]; 
                    $total9anos += $dados[$j]["qtd9anos"];
                    $total10anos += $dados[$j]["qtd10anos"];
                    $total11anos += $dados[$j]["qtd11anos"];
                    $total12anos += $dados[$j]["qtd12anos"];
                    $totalMais12anos += $dados[$j]["qtdmais12anos"];
                    $totalIdadeSuperior += $idadeSuperior;
                    
                    echo "<tr height=\"30px\" align=\"center\">
                            <td bgcolor=\"#f0f0f0\">
                                <b>".$serie[$i]["scedescricao"]."</b>
                                <input type=\"hidden\" name=\"serie_id[$y][".$serie[$i]["sceid"]."]\" value=\"".$serie[$i]["sceid"]."\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"matfinal[$y][".$serie[$i]["sceid"]."]\" name=\"matfinal[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["matfinal"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td> 
                                                                                                  
                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd7anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd7anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd7anos"]."\" onkeyup=\"calculaDistorcao();   calcularSubtotal($y);\" onkeypress=\"return somenteNumeros(event);\">
                            </td>

                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd8anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd8anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd8anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y);\"  onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                             
                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd9anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd9anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd9anos"]."\" onkeyup=\"calculaDistorcao();  calcularSubtotal($y);\"  onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                             
                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd10anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd10anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd10anos"]."\" onkeyup=\" calculaDistorcao();   calcularSubtotal($y);\" onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                              
                            <td bgcolor=\"#f0f0f0\">                                                           
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd11anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd11anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd11anos"]."\" onkeyup=\"calculaDistorcao();  calcularSubtotal($y);\" onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                              
                            <td bgcolor=\"#f0f0f0\">                                                           
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd12anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y);\"  onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtdmais12anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtdmais12anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtdmais12anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\" id=\"idade_superior[$y][".$serie[$i]["sceid"]."]\">
                                ".$idadeSuperior."                     
                            </td>                                      
                            <td bgcolor=\"#f0f0f0\" id=\"taxa_distorcao[$y][".$serie[$i]["sceid"]."]\">
                                ".$taxaDistorcao."
                            </td>
                          </tr>";
                }
            }
           echo "<tr height=\"30px\" align=\"center\">
                        <td bgcolor=\"#DADADA\"><b>Subtotal</b></td>
                        <td id=\"subTotalMatFinal[$y]\" bgcolor=\"#DADADA\">".$subTotalMatFinal[$y]."</td>
                        <td id=\"subTotal7anos[$y]\" bgcolor=\"#DADADA\">".$subTotal7anos[$y]."</td>
                        <td id=\"subTotal8anos[$y]\" bgcolor=\"#DADADA\">".$subTotal8anos[$y]."</td>
                        <td id=\"subTotal9anos[$y]\" bgcolor=\"#DADADA\">".$subTotal9anos[$y]."</td>
                        <td id=\"subTotal10anos[$y]\" bgcolor=\"#DADADA\">".$subTotal10anos[$y]."</td>
                        <td id=\"subTotal11anos[$y]\" bgcolor=\"#DADADA\">".$subTotal11anos[$y]."</td>
                        <td id=\"subTotal12anos[$y]\" bgcolor=\"#DADADA\">".$subTotal12anos[$y]."</td>
                        <td id=\"subTotalMais12anos[$y]\" bgcolor=\"#DADADA\">".$subTotalMais12anos[$y]."</td>
                        <td id=\"subTotalIdadeSuperior[$y]\" bgcolor=\"#DADADA\">".$subTotalIdadeSuperior[$y]."</td>
                        <td id=\"subTotalTaxaDistorcao[$y]\" bgcolor=\"#DADADA\">".$subTotalTaxaDistorcao[$y]."%</td>
                  </tr>";
        ?>
    </tbody>
    
<?php


}
//end of turn´s for

 
            if($totalMatFinal != 0)
            {
                $totalTaxa = str_replace('.',',',number_format((($totalIdadeSuperior / $totalMatFinal) * 100),2));
				$totalTaxaDistorcao = round( $totalTaxa );
            }
            else
            {
            	$totalTaxaDistorcao = 0;
            }
                
            echo "<tr height=\"30px\" align=\"center\">
                        <td bgcolor=\"gray\"><b>TOTAL</b></td>
                        <td id=\"totalMatFinal\" bgcolor=\"gray\">".$totalMatFinal."</td>
                        <td id=\"total7anos\" bgcolor=\"gray\">".$total7anos."</td>
                        <td id=\"total8anos\" bgcolor=\"gray\">".$total8anos."</td>
                        <td id=\"total9anos\" bgcolor=\"gray\">".$total9anos."</td>
                        <td id=\"total10anos\" bgcolor=\"gray\">".$total10anos."</td>
                        <td id=\"total11anos\" bgcolor=\"gray\">".$total11anos."</td>
                        <td id=\"total12anos\" bgcolor=\"gray\">".$total12anos."</td>
                        <td id=\"totalMais12anos\" bgcolor=\"gray\">".$totalMais12anos."</td>
                        <td id=\"totalIdadeSuperior\" bgcolor=\"gray\">".$totalIdadeSuperior."</td>
                        <td id=\"totalTaxaDistorcao\" bgcolor=\"gray\">".$totalTaxaDistorcao."%</td>
                  </tr>";
                  ?>
 </table>
    <table class="tabela" bgcolor="#f5f5f5" align="center">
    <?php echo $cForm->montarbuttons("submeteDadosAproveitamento_p96()");?>
    </table>
</form>

<script language="javascript" type="text/javascript"><!--

function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A";
}

function submeteDadosAproveitamento_p96() {
	document.getElementById('p96_form_aproveitamento').submit();
	return true;
}
 
 

function carregaValores() 
{	
    var soma = 0;  
    for(var h=0; h<=3; h++) 
    {       
        for(var i=1; i<=5; i++) 
        {            
            // alert(document.getElementById('matfinal['+h+']['+series[i]+']').value);
            soma += Number(document.getElementById('matfinal['+h+']['+i+']').value); 
        } 
    } 
    document.getElementById('totalMatFinal').innerHTML = soma;
   
     
    //teste pedro
    var tstsoma8 = 0;
    var valor8 = 0;  
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 1; b<= 5; b++ )
        {  
            valor8 = Number(document.getElementById('qtd8anos['+a+']['+b+']').value);
            tstsoma8 += valor8;  
        }
    } 
    document.getElementById('total8anos').innerHTML = tstsoma8;
    
    
    var tstsoma9 = 0;
    var valor9 = 0;  
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 1; b<= 5; b++ )
        { 
            valor9 = Number(document.getElementById('qtd9anos['+a+']['+b+']').value);
            tstsoma9 += valor9; 
        }
    } 
    document.getElementById('total9anos').innerHTML = tstsoma9;
     
    var tstsoma10 = 0;
    var valor10 = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 1; b<= 5; b++ )
        { 
            valor10 = Number(document.getElementById('qtd10anos['+a+']['+b+']').value);
            tstsoma10 += valor10;  
        }
    } 
    document.getElementById('total10anos').innerHTML = tstsoma10;
    
    var tstsoma11 = 0;
    var valor11 = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 1; b<= 5; b++ )
        { 
            valor11 = Number(document.getElementById('qtd11anos['+a+']['+b+']').value);
            tstsoma11 += valor11;  
        }
    }
    
    document.getElementById('total11anos').innerHTML = tstsoma11; 
    
    var tstsoma12 = 0;
    var valor12 = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 1; b<= 5; b++ )
        { 
            valor12 = Number(document.getElementById('qtd12anos['+a+']['+b+']').value);
            tstsoma12 += valor12;  
        }
    } 
    document.getElementById('total12anos').innerHTML = tstsoma12;
     
    var tstsoma12_ = 0;
    var valor12_ = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 1; b<= 5; b++ )
        { 
            valor12_ = Number(document.getElementById('qtdmais12anos['+a+']['+b+']').value);
            tstsoma12_ += valor12_;  
        }
    } 
    document.getElementById('totalMais12anos').innerHTML = tstsoma12_;
    
    var totalIdadeMaior;    
    
    totalIdadeMaior = Number(tstsoma12_ + tstsoma12 + tstsoma11 + tstsoma10 + tstsoma9 + tstsoma8);
    document.getElementById('totalIdadeSuperior').innerHTML = totalIdadeMaior;
    
    
}


function calculaDistorcao()
{

var taxaDistorcao = 0;
var matFinal = 0;
var idadeSuperior = 0;
	for( var i = 0; i <= 3; i++ )
    {
        for( var serie_campo = 1; serie_campo<= 5; serie_campo++ )
        { 
//        	//document.getElementById('qtd7anos['+i+']['+j+']').disabled = 1;
//        	document.getElementById('qtd7anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        	 matFinal =  Number(document.getElementById('matfinal['+i+']['+serie_campo+']').value)
        	 
        	 if( serie_campo == 1 )
        	 {
	             var idadeSuperior =  (
				 Number(document.getElementById('qtd8anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd9anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd10anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd11anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd12anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais12anos['+i+']['+serie_campo+']').value));
 			}
 			else if( serie_campo == 2 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd9anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd10anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd11anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd12anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais12anos['+i+']['+serie_campo+']').value));
 			}
 			else if( serie_campo == 3 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd10anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd11anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd12anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais12anos['+i+']['+serie_campo+']').value));
 			}
 			else if( serie_campo == 4 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd11anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd12anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais12anos['+i+']['+serie_campo+']').value));
 			}
 			else if( serie_campo == 5 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd12anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais12anos['+i+']['+serie_campo+']').value));
 			}
 			
	        document.getElementById('idade_superior['+i+']['+serie_campo+']').innerHTML = idadeSuperior;
	        
	        var taxaDistorcao = Number(idadeSuperior / matFinal) * 100;    
	        
	        if(isNaN(taxaDistorcao))
	        { 
	        	 taxaDistorcao = '0%';   
	        }
	        else
	        {
	        	taxaDistorcao = taxaDistorcao.toFixed().replace('.',',') + "%";
	       	} 
	        
	        	        
	        document.getElementById('taxa_distorcao['+i+']['+serie_campo+']').innerHTML = taxaDistorcao;
	        
        }
    }
    calculaTotalGeral(); 
}

function bloqueiaCampos()
{
	for( var i = 0; i <= 3; i++ )
    {
        for( var j = 1; j<= 5; j++ )
        { 
        	//document.getElementById('qtd7anos['+i+']['+j+']').disabled = 1;
        	document.getElementById('qtd7anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    }
	for( var i = 0; i <= 3; i++ )
    {
        for( var j = 2; j<= 5; j++ )
        { 
        	//document.getElementById('qtd8anos['+i+']['+j+']').disabled = 1;
        	document.getElementById('qtd8anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    for( var i = 0; i <= 3; i++ )
    {
        for( var j = 3; j<= 5; j++ )
        { 
        	//document.getElementById('qtd9anos['+i+']['+j+']').disabled = 1;
        	document.getElementById('qtd9anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    for( var i = 0; i <= 3; i++ )
    {
        for( var j = 4; j<= 5; j++ )
        { 
        	//document.getElementById('qtd10anos['+i+']['+j+']').disabled = 1;
        	document.getElementById('qtd10anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    for( var i = 0; i <= 3; i++ )
    {
        for( var j = 5; j<= 5; j++ )
        { 
        	//document.getElementById('qtd11anos['+i+']['+j+']').disabled = 1;
        	document.getElementById('qtd11anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
     
}

function calcularSubtotal(turno)
{  
	var subMatFinal	 = 0;
	var subQtd7anos	 = 0;
	var subQtd8anos  = 0;
	var subQtd9anos  = 0;
	var subQtd10anos = 0;
	var subQtd11anos = 0;
	var subQtd12anos = 0;
	var subQtdMais12anos  = 0; 
	var subTotalMatFinal  = 0;
	var subTotalQtd7anos  = 0;
	var subTotalQtd8anos  = 0;
	var subTotalQtd9anos  = 0;
	var subTotalQtd10anos = 0;
	var subTotalQtd11anos = 0;
	var subTotalQtd12anos = 0;
	var subTotalQtdMais12anos = 0;
	var somaSubTotalIdadeSuperior = 0;
	var subTotalIdadeSuperior = 0;
	var subTotalTaxaDistorcao = 0;
	
		for( var serie = 1; serie<=5; serie++ )
		{	 
			 subMatFinal = Number(document.getElementById('matfinal['+turno+']['+serie+']').value);
			 subTotalMatFinal += subMatFinal;
						
			 subQtd7anos = Number(document.getElementById('qtd7anos['+turno+']['+serie+']').value);	
			 subTotalQtd7anos += subQtd7anos;
				
			 subQtd8anos = Number(document.getElementById('qtd8anos['+turno+']['+serie+']').value);	
			 subTotalQtd8anos += subQtd8anos;

			 subQtd9anos = Number(document.getElementById('qtd9anos['+turno+']['+serie+']').value);	
			 subTotalQtd9anos += subQtd9anos;
				
			 subQtd10anos = Number(document.getElementById('qtd10anos['+turno+']['+serie+']').value);	
			 subTotalQtd10anos += subQtd10anos;
			
			 subQtd11anos = Number(document.getElementById('qtd11anos['+turno+']['+serie+']').value);	
			 subTotalQtd11anos += subQtd11anos;
			
			 subQtd12anos = Number(document.getElementById('qtd12anos['+turno+']['+serie+']').value);	
			 subTotalQtd12anos += subQtd12anos;
			
			 subQtdMais12anos = Number(document.getElementById('qtdmais12anos['+turno+']['+serie+']').value);	
	 	   	 subTotalQtdMais12anos += subQtdMais12anos;
	 	   	 
	 	   	 //______________
	 	   	 
	 	   	 somaSubTotalIdadeSuperior += Number( document.getElementById('idade_superior['+turno+']['+serie+']').innerHTML);
	 	   //	 alert( document.getElementById('idade_superior['+turno+']['+serie+']').value );
	 	} 
		 
		document.getElementById('subTotalMatFinal['+turno+']').innerHTML = subTotalMatFinal;
		document.getElementById('subTotal7anos['+turno+']').innerHTML = subTotalQtd7anos;
		document.getElementById('subTotal8anos['+turno+']').innerHTML = subTotalQtd8anos;
		document.getElementById('subTotal9anos['+turno+']').innerHTML = subTotalQtd9anos;
		document.getElementById('subTotal10anos['+turno+']').innerHTML = subTotalQtd10anos;
		document.getElementById('subTotal11anos['+turno+']').innerHTML = subTotalQtd11anos;
		document.getElementById('subTotal12anos['+turno+']').innerHTML = subTotalQtd12anos;
		document.getElementById('subTotalMais12anos['+turno+']').innerHTML = subTotalQtdMais12anos;
		
	 	
		document.getElementById('subTotalIdadeSuperior['+turno+']').innerHTML = somaSubTotalIdadeSuperior;
		 
		 
		subTotalTaxaDistorcao = Number(somaSubTotalIdadeSuperior / subTotalMatFinal ) * 100;
		
		if(isNaN(subTotalTaxaDistorcao))
		{
			subTotalTaxaDistorcao = '0%';
		}
		else
		{ 
			subTotalTaxaDistorcao = subTotalTaxaDistorcao.toFixed().replace('.',',')+'%';
		}
		document.getElementById('subTotalTaxaDistorcao['+turno+']').innerHTML = subTotalTaxaDistorcao;
			
		 
}

function calculaTotalGeral()
{
	var somaSubtotaisIdadeSuperior = 0;
	var somaSubMatFinal	 = 0;
	var somaSubQtd7anos	 = 0;
	var somaSubQtd8anos  = 0;
	var somaSubQtd9anos  = 0;
	var somaSubQtd10anos = 0;
	var somaSubQtd11anos = 0;
	var somaSubQtd12anos = 0;
	var somaSubQtdMais12anos  = 0; 
	var somaSubTotalMatFinal  = 0;
	var totalTaxaDistorcao  = 0;
	
	for ( var turno = 0; turno <= 3; turno++ )
	{
		 
			somaMatFinal = Number(document.getElementById('subTotalMatFinal['+turno+']').innerHTML);	
			somaSubMatFinal += somaMatFinal;
			 
			somaQtd7anos = Number(document.getElementById('subTotal7anos['+turno+']').innerHTML);	
			somaSubQtd7anos += somaQtd7anos;
			
			somaQtd8anos = Number(document.getElementById('subTotal8anos['+turno+']').innerHTML);	
			somaSubQtd8anos += somaQtd8anos;
			
			somaQtd9anos = Number(document.getElementById('subTotal9anos['+turno+']').innerHTML);	
			somaSubQtd9anos += somaQtd9anos;
			
			somaQtd10anos = Number(document.getElementById('subTotal10anos['+turno+']').innerHTML);	
			somaSubQtd10anos += somaQtd10anos;
			
			somaQtd11anos = Number(document.getElementById('subTotal11anos['+turno+']').innerHTML);	
			somaSubQtd11anos += somaQtd11anos;
			
			somaQtd12anos = Number(document.getElementById('subTotal12anos['+turno+']').innerHTML);	
			somaSubQtd12anos += somaQtd12anos;
			
			somaQtdMais12anos = Number(document.getElementById('subTotalMais12anos['+turno+']').innerHTML);	
			somaSubQtdMais12anos += somaQtdMais12anos;
			
			somaTotalIdadeSuperior= Number( document.getElementById('subTotalIdadeSuperior['+turno+']').innerHTML);
		 	somaSubtotaisIdadeSuperior += somaTotalIdadeSuperior;
	}
	
	document.getElementById('totalMatFinal').innerHTML   = somaSubMatFinal;
	document.getElementById('total7anos').innerHTML 	 = somaSubQtd7anos;
	document.getElementById('total8anos').innerHTML 	 = somaSubQtd8anos;
	document.getElementById('total9anos').innerHTML 	 = somaSubQtd9anos;
	document.getElementById('total10anos').innerHTML	 = somaSubQtd10anos;
	document.getElementById('total11anos').innerHTML 	 = somaSubQtd11anos;
	document.getElementById('total12anos').innerHTML 	 = somaSubQtd12anos;
	document.getElementById('totalMais12anos').innerHTML = somaSubQtdMais12anos;
	document.getElementById('totalIdadeSuperior').innerHTML = somaSubtotaisIdadeSuperior;
	
	totalTaxaDistorcao = Math.floor(Number( somaSubtotaisIdadeSuperior / somaSubMatFinal) * 100);

	if(isNaN(totalTaxaDistorcao))
	{
		totalTaxaDistorcao = '0%';
	}
	else
	{	
		totalTaxaDistorcao = totalTaxaDistorcao.toFixed().replace('.',',')+'%';
	}
	
	document.getElementById('totalTaxaDistorcao').innerHTML = totalTaxaDistorcao; 


}
  
bloqueiaCampos(); 
calculaDistorcao();
calcularSubtotal(0);
calcularSubtotal(1);
calcularSubtotal(2);
calcularSubtotal(3);
carregaValores();
calculaTotalGeral();
--></script>
