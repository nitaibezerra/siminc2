<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

function salva(){
	global $db,$pdeid;
    
    
    for( $y = 0; $y < count( $_POST["serie_id"]); $y++ )
    {
        $id_turno = ($y + 1);
        
        
        if($_POST["submetido"]) 
        {
            $pdeid         = $_POST["pdeid"];
            $serieId       = $_POST["serie_id"];
            $matfinal      = $_POST["matfinal"];
            $qtd12anos     = $_POST["qtd12anos"];
            $qtd13anos     = $_POST["qtd13anos"];
            $qtd14anos     = $_POST["qtd14anos"];
            $qtd15anos     = $_POST["qtd15anos"];
            $qtd16anos     = $_POST["qtd16anos"];
            $qtdmais16anos = $_POST["qtdmais16anos"];
 
            for($i=6; $i <= (count($serieId[$y]) + 5); $i++) 
            {
                $matfinal[$y][$i]      = ($matfinal[$y][$i] == NULL) ? 'NULL' : $matfinal[$y][$i];
                $qtd12anos[$y][$i]     = ($qtd12anos[$y][$i] == NULL) ? 'NULL' : $qtd12anos[$y][$i];
                $qtd13anos[$y][$i]     = ($qtd13anos[$y][$i] == NULL) ? 'NULL' : $qtd13anos[$y][$i];
                $qtd14anos[$y][$i]     = ($qtd14anos[$y][$i] == NULL) ? 'NULL' : $qtd14anos[$y][$i];
                $qtd15anos[$y][$i]     = ($qtd15anos[$y][$i] == NULL) ? 'NULL' : $qtd15anos[$y][$i];
                $qtd16anos[$y][$i]     = ($qtd16anos[$y][$i] == NULL) ? 'NULL' : $qtd16anos[$y][$i];
                $qtdmais16anos[$y][$i] = ($qtdmais16anos[$y][$i] == NULL) ? 'NULL' : $qtdmais16anos[$y][$i];
            
                 for( $k=5; $k <= 11; $k++)
                 {
                    
                    if($k == 6) 
                    {                        
                        continue;
                    }
           
                    $existe = $db->pegaUm("SELECT disid FROM pdeescola.distorcaoidadeserie
                                                   WHERE pdeid = ".$pdeid."
                                                     AND sceid = ".$serieId[$y][$i]."
                                                     AND disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA."
                                                     AND faeid = ".$k."
                                                     AND titid = ".$id_turno);
                    
                     
                    
                      
                    if($k == 5) { $qtdAlunos  =  $qtd12anos[$y][$i]; } 
                    if($k == 7) { $qtdAlunos  =  $qtd13anos[$y][$i]; }
                    if($k == 8) { $qtdAlunos  =  $qtd14anos[$y][$i]; }
                    if($k == 9) { $qtdAlunos  =  $qtd15anos[$y][$i]; }
                    if($k == 10) { $qtdAlunos =  $qtd16anos[$y][$i]; }
                    if($k == 11) { $qtdAlunos =  $qtdmais16anos[$y][$i]; }		
          
 
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
                                               '{$i}',
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
								  	trim(epfnomearquivo) = 'p10_6' AND
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
												 	ppritem = 'p10_6'");
			if( $existe_preenchimento == NULL )
			{
				$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p10_6',  '{$_REQUEST['anoreferencia']}')";
				$db->executar( $sql_p );
		 
			}
		$db->commit();  
        }
    }  //die();
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
<?=subCabecalho('10.6 - Distorção idade-série - 5ª - 8ª série/ 6º - 9º ano do Ensino Fundamental (ano anterior)'); ?>
<form method="post" id="p97_form_aproveitamento" name="p97_form_aproveitamento">
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
    <td colspan="10">
    <div class="SubtituloEsquerda" style="padding: 5px; text-align: left; "><?php echo $turno[$y]['titdescricao']; ?></div>
    </td>
    </thead>
    <thead>
        <tr>
            <th width="5%">SÉRIE/ANO</th>
            <th width="10%">Matrícula Final (A)</th>
            <th width="10%">Até 12 anos</th>
            <th width="10%">Até 13 anos</th>
            <th width="10%">Até 14 anos</th>
            <th width="10%">Até 15 anos</th>
            <th width="10%">Até 16 anos</th>
            <th width="10%">Mais de 16 anos</th>
            <th width="15%">Total de alunos com idade superior à série respectiva (B)</th>
            <th width="10%">Taxa de Distorção (B/A) x 100</th>
        </tr>
    </thead>
    <tbody>
        <?php
            $serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
                                    WHERE tmeid = 3 AND scetipo = 'S'
                                    ORDER BY sceid");
            /*
            echo("<pre>");
        	print_r( $serie );
         	echo("</pre>");
            //die();
            */
                        
            $totalMatFinal = 0;
            $total12anos = 0;
            $total13anos = 0;
            $total14anos = 0;
            $total15anos = 0;
            $total16anos = 0;
            $totalMais16anos = 0;
            $totalIdadeSuperior = 0;
            
            for($i=0; $i<count($serie); $i++) {
                echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
                
                $dados = $db->carregar("SELECT
                                            matfinal,
                                            sum(qtdalunos1) as qtd12anos,
                                            sum(qtdalunos2) as qtd13anos,
                                            sum(qtdalunos3) as qtd14anos,
                                            sum(qtdalunos4) as qtd15anos,
                                            sum(qtdalunos5) as qtd16anos,
                                            sum(qtdalunos6) as qtdmais16anos
                                        FROM
                                        (
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
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
                                            dis.faeid = 5 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
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
                                            dis.faeid = 7 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
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
                                            dis.faeid = 8 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
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
                                            dis.faeid = 9 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
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
                                            dis.faeid = 10 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
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
                                            dis.faeid = 11 AND
                                            titid = '{$turno[$y]['titid']}'
                                        ) as foo 
                                        GROUP BY matfinal");
                
                for($j=0; $j<count($dados); $j++) {
                    $idadeSuperior = ($dados[$j]["qtd13anos"] + $dados[$j]["qtd14anos"]
                                      + $dados[$j]["qtd15anos"] + $dados[$j]["qtd16anos"] + $dados[$j]["qtdmais16anos"]);
                    $idadeSuperior = ($idadeSuperior == 0) ? '' : $idadeSuperior;
                                      
                    if($dados[$j]["matfinal"] != 0)
                    {
                        $taxa = str_replace('.',',',number_format((($idadeSuperior / $dados[$j]["matfinal"]) * 100),2))."%";
                    	$taxaDistorcao = floor( $taxa ).'%';
                    }
                    else
                    {
                    	$taxaDistorcao = "";
                    }
                     
                    $totalMatFinal += $dados[$j]["matfinal"];
                    $total12anos += $dados[$j]["qtd12anos"];
                    $total13anos += $dados[$j]["qtd13anos"];
                    $total14anos += $dados[$j]["qtd14anos"];
                    $total15anos += $dados[$j]["qtd15anos"];
                    $total16anos += $dados[$j]["qtd16anos"];
                    $totalMais16anos += $dados[$j]["qtdmais16anos"];
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
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd12anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd13anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd13anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd13anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd14anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd14anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd14anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd15anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd15anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd15anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd16anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd16anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd16anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtdmais16anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtdmais16anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtdmais16anos"]."\" onkeyup=\" calculaDistorcao(); calcularSubtotal($y); \" onkeypress=\"return somenteNumeros(event);\">
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
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"subTotalMatFinal[$y]\" bgcolor=\"#DADADA\">".$totalMatFinal."</td>
					<td id=\"subTotal12anos[$y]\" bgcolor=\"#DADADA\">".$total12anos."</td>
					<td id=\"subTotal13anos[$y]\" bgcolor=\"#DADADA\">".$total13anos."</td>
					<td id=\"subTotal14anos[$y]\" bgcolor=\"#DADADA\">".$total14anos."</td>
					<td id=\"subTotal15anos[$y]\" bgcolor=\"#DADADA\">".$total15anos."</td>
					<td id=\"subTotal16anos[$y]\" bgcolor=\"#DADADA\">".$total16anos."</td>
					<td id=\"subTotalMais16anos[$y]\" bgcolor=\"#DADADA\">".$totalMais16anos."</td>
					<td id=\"subTotalIdadeSuperior[$y]\" bgcolor=\"#DADADA\">".$totalIdadeSuperior."</td>
					<td id=\"subTotalTaxaDistorcao[$y]\" bgcolor=\"#DADADA\">".$totalTaxaDistorcao."%</td>
			  </tr>";
}//end of turn´s for

		if($totalMatFinal != 0)
		{
			$totalTaxa = str_replace('.',',',number_format((($totalIdadeSuperior / $totalMatFinal) * 100),2));
			$totalTaxaDistorcao = floor( $totalTaxa );
		}
		else
		{
			$totalTaxaDistorcao = 0;
		}
			
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"gray\"><b>TOTAL</b></td>
					<td id=\"totalMatFinal\" bgcolor=\"gray\">".$totalMatFinal."</td>
					<td id=\"total12anos\" bgcolor=\"gray\">".$total12anos."</td>
					<td id=\"total13anos\" bgcolor=\"gray\">".$total13anos."</td>
					<td id=\"total14anos\" bgcolor=\"gray\">".$total14anos."</td>
					<td id=\"total15anos\" bgcolor=\"gray\">".$total15anos."</td>
					<td id=\"total16anos\" bgcolor=\"gray\">".$total16anos."</td>
					<td id=\"totalMais16anos\" bgcolor=\"gray\">".$totalMais16anos."</td>
					<td id=\"totalIdadeSuperior\" bgcolor=\"gray\">".$totalIdadeSuperior."</td>
					<td id=\"totalTaxaDistorcao\" bgcolor=\"gray\">".$totalTaxaDistorcao."%</td>
			  </tr>";
	?>
</tbody>
<? $cForm->montarbuttons("submeteDadosAproveitamento_p97()");?>				
</table>
</form>

<script language="javascript" type="text/javascript">

function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A";
}

function submeteDadosAproveitamento_p97() {
	document.getElementById('p97_form_aproveitamento').submit();
	return true;
}


function carregaValores() 
{	
    var soma = 0;  
    for(var h=0; h<=3; h++) 
    {       
        for(var i=6; i<=9; i++) 
        {            
            // alert(document.getElementById('matfinal['+h+']['+series[i]+']').value);
            soma += Number(document.getElementById('matfinal['+h+']['+i+']').value); 
        } 
    } 
    document.getElementById('totalMatFinal').innerHTML = soma;
   
     
    //teste pedro
    var tstsoma12 = 0;
    var valor12 = 0;  
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 6; b<= 9; b++ )
        {  
            valor12 = Number(document.getElementById('qtd12anos['+a+']['+b+']').value);
            tstsoma12 += valor12;  
        }
    } 
    document.getElementById('total12anos').innerHTML = tstsoma12;
    
    
    var tstsoma13 = 0;
    var valor13 = 0;  
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 6; b<= 9; b++ )
        { 
            valor13 = Number(document.getElementById('qtd13anos['+a+']['+b+']').value);
            tstsoma13 += valor13; 
        }
    } 
    document.getElementById('total13anos').innerHTML = tstsoma13;
     
    var tstsoma14 = 0;
    var valor14 = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 6; b<= 9; b++ )
        { 
            valor14 = Number(document.getElementById('qtd14anos['+a+']['+b+']').value);
            tstsoma14 += valor14;  
        }
    } 
    document.getElementById('total14anos').innerHTML = tstsoma14;
    
    tstsoma15 = 0;
    valor15 = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 6; b<= 9; b++ )
        { 
            valor15 = Number(document.getElementById('qtd15anos['+a+']['+b+']').value);
            tstsoma15 += valor15;  
        }
    }
 
    document.getElementById('total15anos').innerHTML = tstsoma15; 
    
    tstsoma16 = 0;
    valor16 = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 6; b<= 9; b++ )
        { 
            valor16 = Number(document.getElementById('qtd16anos['+a+']['+b+']').value);
            tstsoma16 += valor16;  
        }
    } 
    document.getElementById('total16anos').innerHTML = tstsoma16;
     
    tstsoma16_ = 0;
    valor16_ = 0; 
    for( var a = 0; a <= 3; a++ )
    {
        for( var b = 6; b<= 9; b++ )
        { 
            valor16_ = Number(document.getElementById('qtdmais16anos['+a+']['+b+']').value);
            tstsoma16_ += valor16_;  
        }
    } 
    document.getElementById('totalMais16anos').innerHTML = tstsoma16_;
    
    var totalIdadeMaior;    
    
    totalIdadeMaior = Number(tstsoma16_ + tstsoma16 + tstsoma15 + tstsoma14 + tstsoma13 + tstsoma12);
    document.getElementById('totalIdadeSuperior').innerHTML = totalIdadeMaior;
    
    
}



function calculaDistorcao()
{

var taxaDistorcao = 0;
var matFinal = 0;
var idadeSuperior = 0;
	for( var i = 0; i <= 3; i++ )
    {
        for( var serie_campo = 6; serie_campo<= 9; serie_campo++ )
        { 
//        	//document.getElementById('qtd7anos['+i+']['+j+']').disabled = 1;
//        	document.getElementById('qtd7anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        	 matFinal =  Number(document.getElementById('matfinal['+i+']['+serie_campo+']').value)
        	 
 			 if( serie_campo == 6 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd13anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd14anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd15anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd16anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais16anos['+i+']['+serie_campo+']').value));
	             
 			}
 			else if( serie_campo == 7 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd14anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd15anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd16anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais16anos['+i+']['+serie_campo+']').value));
 			}
 			else if( serie_campo == 8 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd15anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtd16anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais16anos['+i+']['+serie_campo+']').value));
 			}
 			else if( serie_campo == 9 )
        	 {
	             var idadeSuperior =  (
	             Number(document.getElementById('qtd16anos['+i+']['+serie_campo+']').value) +
	             Number(document.getElementById('qtdmais16anos['+i+']['+serie_campo+']').value));
 			}
 			
	        document.getElementById('idade_superior['+i+']['+serie_campo+']').innerHTML = idadeSuperior;
	        
	        var taxaDistorcao = Math.floor(Number(idadeSuperior / matFinal) * 100);    
	        
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
        for( var j = 6; j<= 9; j++ )
        { 
        	 document.getElementById('qtd12anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    for( var i = 0; i <= 3; i++ )
    {
        for( var j = 7; j<= 9; j++ )
        { 
        	 document.getElementById('qtd13anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    for( var i = 0; i <= 3; i++ )
    {
        for( var j = 8; j<= 9; j++ )
        { 
        	 document.getElementById('qtd14anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    for( var i = 0; i <= 3; i++ )
    {
        for( var j = 9; j<= 9; j++ )
        { 
        	 document.getElementById('qtd15anos['+i+']['+j+']').style.background = 'silver';        	
        	 
        }
    } 
    
}



function calcularSubtotal(turno)
{  
	var subMatFinal	 = 0;
	var subQtd12anos	 = 0;
	var subQtd13anos  = 0;
	var subQtd14anos  = 0;
	var subQtd15anos = 0;
	var subQtd16anos = 0;
	var subQtdMais16anos  = 0; 
	var subTotalMatFinal  = 0;
	var subTotalQtd12anos  = 0;
	var subTotalQtd13anos  = 0;
	var subTotalQtd14anos  = 0;
	var subTotalQtd15anos = 0;
	var subTotalQtd16anos = 0;
	var subTotalQtdMais16anos = 0;
	var somaSubTotalIdadeSuperior = 0;
	var subTotalIdadeSuperior = 0;
	var subTotalTaxaDistorcao = 0;
	
		for( var serie = 6; serie<=9; serie++ )
		{	 
			 subMatFinal = Number(document.getElementById('matfinal['+turno+']['+serie+']').value);
			 subTotalMatFinal += subMatFinal;
						
			 subQtd12anos = Number(document.getElementById('qtd12anos['+turno+']['+serie+']').value);	
			 subTotalQtd12anos += subQtd12anos;
				
			 subQtd13anos = Number(document.getElementById('qtd13anos['+turno+']['+serie+']').value);	
			 subTotalQtd13anos += subQtd13anos;

			 subQtd14anos = Number(document.getElementById('qtd14anos['+turno+']['+serie+']').value);	
			 subTotalQtd14anos += subQtd14anos;
				
			 subQtd15anos = Number(document.getElementById('qtd15anos['+turno+']['+serie+']').value);	
			 subTotalQtd15anos += subQtd15anos;
			
			 subQtd16anos = Number(document.getElementById('qtd16anos['+turno+']['+serie+']').value);	
			 subTotalQtd16anos += subQtd16anos;
			
			 subQtdMais16anos = Number(document.getElementById('qtdmais16anos['+turno+']['+serie+']').value);	
	 	   	 subTotalQtdMais16anos += subQtdMais16anos;
	 	   	 
	 	   	 //______________
	 	   	 
	 	   	 somaSubTotalIdadeSuperior += Number( document.getElementById('idade_superior['+turno+']['+serie+']').innerHTML);
	 	   //	 alert( document.getElementById('idade_superior['+turno+']['+serie+']').value );
	 	} 
		 
		document.getElementById('subTotalMatFinal['+turno+']').innerHTML = subTotalMatFinal;
		document.getElementById('subTotal12anos['+turno+']').innerHTML = subTotalQtd12anos;
		document.getElementById('subTotal13anos['+turno+']').innerHTML = subTotalQtd13anos;
		document.getElementById('subTotal14anos['+turno+']').innerHTML = subTotalQtd14anos;
		document.getElementById('subTotal15anos['+turno+']').innerHTML = subTotalQtd15anos; 
		document.getElementById('subTotal16anos['+turno+']').innerHTML = subTotalQtd16anos;
		document.getElementById('subTotalMais16anos['+turno+']').innerHTML = subTotalQtdMais16anos;
		
	 	
		document.getElementById('subTotalIdadeSuperior['+turno+']').innerHTML = somaSubTotalIdadeSuperior;
		 
		 
		subTotalTaxaDistorcao = Math.floor(Number(somaSubTotalIdadeSuperior / subTotalMatFinal ) * 100);
		
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
	var somaSubQtd12anos	 = 0;
	var somaSubQtd13anos  = 0;
	var somaSubQtd14anos  = 0;
	var somaSubQtd15anos = 0;
	var somaSubQtd16anos = 0;
	var somaSubQtdMais16anos  = 0; 
	var somaSubTotalMatFinal  = 0;
	var totalTaxaDistorcao  = 0;
	
	for ( var turno = 0; turno <= 3; turno++ )
	{
		 
			somaMatFinal = Number(document.getElementById('subTotalMatFinal['+turno+']').innerHTML);	
			somaSubMatFinal += somaMatFinal;
			 
			somaQtd12anos = Number(document.getElementById('subTotal12anos['+turno+']').innerHTML);	
			somaSubQtd12anos += somaQtd12anos;
			
			somaQtd13anos = Number(document.getElementById('subTotal13anos['+turno+']').innerHTML);	
			somaSubQtd13anos += somaQtd13anos;
			
			somaQtd14anos = Number(document.getElementById('subTotal14anos['+turno+']').innerHTML);	
			somaSubQtd14anos += somaQtd14anos;
			
			somaQtd15anos = Number(document.getElementById('subTotal15anos['+turno+']').innerHTML);	
			somaSubQtd15anos += somaQtd15anos;
			
			somaQtd16anos = Number(document.getElementById('subTotal16anos['+turno+']').innerHTML);	
			somaSubQtd16anos += somaQtd16anos;
			
	 		somaQtdMais16anos = Number(document.getElementById('subTotalMais16anos['+turno+']').innerHTML);	
			somaSubQtdMais16anos += somaQtdMais16anos;
			
			somaTotalIdadeSuperior= Number( document.getElementById('subTotalIdadeSuperior['+turno+']').innerHTML);
		 	somaSubtotaisIdadeSuperior += somaTotalIdadeSuperior;
	}
	
	document.getElementById('totalMatFinal').innerHTML   = somaSubMatFinal;
	document.getElementById('total12anos').innerHTML 	 = somaSubQtd12anos;
	document.getElementById('total13anos').innerHTML 	 = somaSubQtd13anos;
	document.getElementById('total14anos').innerHTML 	 = somaSubQtd14anos;
	document.getElementById('total15anos').innerHTML	 = somaSubQtd15anos;
	document.getElementById('total16anos').innerHTML 	 = somaSubQtd16anos;
	document.getElementById('totalMais16anos').innerHTML = somaSubQtdMais16anos;
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
</script>