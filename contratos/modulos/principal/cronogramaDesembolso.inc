<?php
$ctrid = $_SESSION['ctrid'];
if($_REQUEST['req'] == 'popup'){
	grafico($ctrid);
	exit;
}
if($_POST['action']){
	$Data = formata_data_sql( $_POST['data'] );

	// Update
	if($_GET['dsbid']){
		$verifcaMes=$db->pegaUm("SELECT count(*) 
										FROM contratos.ctdesembolso 
										WHERE dsbstatus='A' 
											AND dsbmesano='".$Data."' 
											AND ctrid=".$_SESSION['ctrid']."
					 						AND dsbid !=".$_GET['dsbid']);
	// Insert
	}else{
		$verifcaMes=$db->pegaUm("SELECT count(*) 
										FROM contratos.ctdesembolso 
										WHERE dsbstatus='A' 
											AND dsbmesano = '".$Data."' 
											AND ctrid=".$_SESSION['ctrid']." ");
	}
	if($verifcaMes==0){
		 salvar();
	}else{
		 die("<script>
				alert('Mês/Ano já está cadastrado para este Contrato! ');
				history.back();
			 </script>");
	}
}
//VERIFICAR A REQUISICAO EDITAR OU APAGAR
switch ( $_REQUEST['requisicao'] ){
	case 'apagar':
		$SQL = "UPDATE contratos.ctdesembolso SET dsbstatus='I' WHERE dsbid={$_GET['dsbid']}";
		$db->executar($SQL);
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/cronogramaDesembolso&acao=A';
			 </script>");
		case 'editar':
			$SQL = "SELECT dsbid, TO_CHAR(dsbmesano, 'MM/YYYY') as mesano, dsbvalor as valor FROM contratos.ctdesembolso  WHERE dsbid={$_GET['dsbid']}";
			$rsDadosContrato = $db->pegaLinha($SQL);
			$vlr_cont					= $rsDadosContrato['valor'];
			$rsDadosContrato['valor'] 	= str_replace(',','.',str_replace('.','',$rsDadosContrato['valor']));
}


function salvar(){
	global $db;
	$formataData = formata_data_sql( $_POST['data'] );
	$formatavalor=str_replace(',','.',str_replace('.','',$_POST['valor']));
	
//dsbid,dsbmesano,dsbvalor FROM contratos.ctdesembolso
	if(!$_GET['dsbid']){

        if ($_POST['repete']) {
            $quebra_data = explode("-",$formataData);
            $mes = $quebra_data[1];
            $ano = $quebra_data[0];

            for ($x = 1; $x <= $_POST['repete']; $x++) {

                $dataNova = $ano . "-" . $mes . "-1";

                $sql = "INSERT INTO
	    		contratos.ctdesembolso
	    		(
					dsbmesano, dsbvalor,ctrid
				)
				VALUES
				(
				'".$dataNova."',".$formatavalor.",".$_SESSION['ctrid']."
				)";
                $db->executar($sql);

                $mes++;
                if ($mes>12){
                    $ano++;
                    $mes=1;
                }
                echo $ano . "-" . $mes . "-1 <br>";
            }
        } else {
            $sql = "INSERT INTO
	    		contratos.ctdesembolso
	    		(
					dsbmesano, dsbvalor,ctrid
				)
				VALUES
				(
				'".$formataData."',".$formatavalor.",".$_SESSION['ctrid']."
				)";
            $db->executar($sql);
        }


	}else{
		$sql = "UPDATE contratos.ctdesembolso
				SET
					dsbmesano='".$formataData."', 
					dsbvalor=".$formatavalor."
				WHERE
					dsbid = ".$_GET['dsbid'];
		$db->executar($sql);
	}
	//dbg($sql,d);
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cronogramaDesembolso', "");
}
//Chamada de cabecalho e abas
global $db;
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );

//montando tabela de cabecalho de contratos com seus valores
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório',false );

if($_SESSION['ctrid']){
	montaCabecalhoContrato( $_SESSION['ctrid'] );
}

// Por padrão os campos vem habilitados
// $desabilitado   = false;
// $somenteLeitura = 'S';
// $verifica = true;
$acesso 		= verificaPermissaoTelaUsuario();
$desabilitado   = $acesso['desabilitado'];
$somenteLeitura = $acesso['leitura'];

$perfis 			= arrayPerfil();
// $arPerfisBloqEdicao = array(PERFIL_CONSULTA_UNIDADE,PERFIL_CONSULTA_GERAL);

// $arInt = array_intersect($perfis, $arPerfisBloqEdicao);

// //MÉTODO PARA A VERIFICAR SE O CONTRATO PERTENCE À UNIDADE DO USUARIO E SE O USUARIO PODERÁ EDITAR NESTA TELA
// if($_SESSION['ctrid'])
// 	$verifica = verificaResponsabilidade($_SESSION['ctrid'],$perfis,array(PERFIL_GESTOR_FINANCEIRO_UNIDADE));
// //dbg(count($arInt),d);
// //verifica se nao esta no bloqueado e se caso nao esteja bloq ele tem direito a editar
// if( count($arInt) != 0 || (count($arInt) == 0 && $verifica==false)) {
// 	$desabilitado 	= true;
// 	$somenteLeitura = 'N';
// }

?>
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script> 
<script src="../includes/Highcharts-3.0.0/js/highcharts.js"></script> 
<form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
	<table class="tabela notprint" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
        	<td class ="SubTituloDireita" align="right">Mês/Ano:</td>
            <td>
                 <?
                 $mesano = ($rsDadosContrato['mesano'] ? $rsDadosContrato['mesano'] : $_POST['mesano']);                               
                 ?>          
                 <?= campo_texto('mesano', 'S', $somenteLeitura, 'Mês/Ano', 5, 7, '##/####', '', '', '',  0, '', '', $mesano); ?> Repetir por  <?= campo_texto('repete', 'N', 'S', 'Repetir Mês', 5, 7, '', '', '', '',  0, '', ''); ?> meses
            </td>
        </tr>	
	    <tr>
           <td class ="SubTituloDireita" align="right" colspan="1">Valor:</td>
           <td>
                <?php 
                $_POST['valor'] = str_replace(',','.',str_replace('.','',$_REQUEST['valor']));
                $valor  = ($vlr_cont ? $vlr_cont : $_POST['valor']);
                if($valor) $valor = number_format($valor,2,",",".");
                ?>
                <?= campo_texto( 'valor', 'S', $somenteLeitura, '', 17, 15, '###.###.###.###,##', '', 'right', '', 0, '','', $valor); ?>
           </td>
       </tr>
       <tr>
          	<td  class ="SubTituloDireita" align="right"></td>
      		<td>
               <input type="hidden" name="action" value="1" id="action">
               <?php $dsbid  = ($rsDadosContrato["dsbid"] ? $rsDadosContrato["dsbid"] : $_GET['dsbid']);
               //dbg($dsbid,d);
               ?>
               <input type="hidden" name="dsbid" id="dsbid" value= "<? echo $dsbid ?>" />
			   <input type="hidden" id="data" name="data" value="<? echo $data ?>" />
			   <input type="hidden" name='valor_formatado' id='valor_formatado' value='<?php echo $rsDadosContrato['valor'];?>'>
               <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();" <?php if($somenteLeitura=="N") echo " disabled"; ?>>
               <input type="button" name="btnGrafico" value="Gráfico" id="btnGrafico" onclick="popupgrafico();">
   
            </td>
       </tr>
	</table>
    <input type="hidden" name="dsbid" value="<?=$_SESSION['dsbid']; ?>" id="dsbid">
</form>

<?php
	$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarDesembolso(\'' || dsbid || '\');\"
		 					title=\"Alterar\">
		 				<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer; margin-left: 3px;\"
		 					onclick=\"javascript: excluirDesembolso(\'' || dsbid || '\');\"
		 					title=\"Excluir\">
		 			  </center>'";
	$sql="SELECT  {$acao} as acao, TO_CHAR(dsbmesano, 'MM/YYYY') as mesano, dsbvalor FROM contratos.ctdesembolso WHERE ctrid=$ctrid AND dsbstatus='A' ORDER BY dsbmesano ASC";
	$dados=$db->carregar($sql);

function tofloat($num) {
    $dotPos = strrpos($num, '.');
    $commaPos = strrpos($num, ',');
    $sep = (($dotPos > $commaPos) && $dotPos) ? $dotPos :
        ((($commaPos > $dotPos) && $commaPos) ? $commaPos : false);

    if (!$sep) {
        return floatval(preg_replace("/[^0-9]/", "", $num));
    }

    return floatval(
        preg_replace("/[^0-9]/", "", substr($num, 0, $sep)) . '.' .
        preg_replace("/[^0-9]/", "", substr($num, $sep+1, strlen($num)))
    );
}
    $soma = 0;
	if($dados){
		foreach($dados as $id=>$valor){
			$dados[$id]['dsbvalor'] = number_format($valor['dsbvalor'],2,",",".");
            $soma += $valor['dsbvalor'];
		}
	}


	//dbg($dados,d);
	
	$cabecalho = array("<center>Ação</center>", "Mês/Ano", "Valor"); 
	$db->monta_lista_array( $dados, $cabecalho, 25, 10, 'N', '', '');
    echo "<p style='margin-left: 62%; margin-top: -25px; font-weight: bold'>" . number_format($soma,2,",",".") . "</p>";
?>

<script type="text/javascript">
		jQuery.noConflict();
function popupgrafico()
{
	var popUp = window.open('/contratos/contratos.php?modulo=principal/cronogramaDesembolso&acao=A&req=popup', 'popupGrafico', 'height=450,width=1450,scrollbars=yes,top=50,left=50');
	popUp.focus();
}
function enviaForm(){
 	var nomeform 				= 'formulario';
	var submeterForm 			= true;
	var campos 					= new Array();
	var tiposDeCampos 			= new Array();

	// Data inputs
	var inputData				= '01/'+jQuery("input[name*='mesano']").val(); 												// De: mes/ano.
	var inputValor				= replaceAll( replaceAll( jQuery("input[name*='valor']").val() , ".", "" ), ",", "");		// De: 333.227.647,05.

	// Dados formatados e atribuidos ao name.
	var campoDataFormatado		= jQuery("#data").val(inputData);				// Para: dia/mes/ano.
	var campoValorFormatado		= jQuery("#valor_formatado").val(inputValor);	// Para: 33322764705.

	// Campos enviados para o validaForm().
	campos[0] 			= "data";
	campos[1]			= "valor_formatado"; 

	// Tipos de dado.
	tiposDeCampos[0] 	= "data"; 
	tiposDeCampos[1] 	= "valor";

	
	validaForm(nomeform, campos, tiposDeCampos, submeterForm ); 

	var res = str.replace("Microsoft", "W3Schools"); 

	
}
function alterarDesembolso( dsbid ){
	location.href = '?modulo=principal/cronogramaDesembolso&acao=<?php echo $_GET['acao'] ?>&dsbid=' + dsbid + '&requisicao=editar';
}

function excluirDesembolso( dsbid ){
	if ( confirm('Deseja apagar este Desembolso?') ){
		location.href = '?modulo=principal/cronogramaDesembolso&acao=<?php echo $_GET['acao'] ?>&dsbid=' + dsbid + '&requisicao=apagar';
	}
}
</script>
<?php 
function grafico($ctrid){
	global $db;	
	$sql="SELECT  TO_CHAR(dsbmesano, 'YYYY/MM') as data, TO_CHAR(dsbmesano, 'MM/YYYY') as mesano,dsbvalor FROM contratos.ctdesembolso WHERE ctrid=$ctrid AND dsbstatus='A' ORDER BY dsbmesano";
	$planejado = $db->carregar ( $sql );
	if($planejado)
		foreach($planejado as $valor){
			$array1[$valor['data']]['planejado']=$valor['dsbvalor'];
			$array1[$valor['data']]['mesano']=$valor['mesano'];
		}
	else
		$array1 = array();
	$sql = "SELECT 
					a.*
				 FROM
				 (
		        		(SELECT  sum(obfvalor) as valorob,
							to_char(obfdatatransacao, 'YYYY/MM') AS data,
		    				to_char(obfdatatransacao, 'MM/YYYY')  as mesano
							
						FROM
							contratos.ctcontrato c
						JOIN contratos.hospital h ON h.hspid = c.hspid
						JOIN contratos.faturacontrato fc ON fc.ctrid = c.ctrid AND fc.ftcstatus = 'A'
						JOIN contratos.ordembancariafatura of ON of.ftcid = fc.ftcid -- AND of.obfsiafi = false
						JOIN contratos.empenhovinculocontrato ec ON ec.epsid = of.epsid AND
																	fc.ctrid = ec.ctrid										 
						JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid																
						WHERE
							fc.ftcstatus = 'A' AND
							fc.ctrid = $ctrid
						group by obfdatatransacao
						ORDER BY
							obfdatatransacao
						
					)UNION ALL(
						SELECT sum(ob.valor) as valorob,
							to_char(ob.datatransacao, 'YYYY/MM') AS data,
							to_char(ob.datatransacao, 'MM/YYYY') as mesano
							
						FROM
							contratos.ctcontrato c
						JOIN entidade.entidade e ON e.entid = c.entidcontratada	
						JOIN contratos.hospital h ON h.hspid = c.hspid
						JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid										 
						JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND es.co_favorecido = e.entnumcpfcnpj																
						JOIN contratos.ob_siafi ob ON ob.empenho = es.nu_empenho  AND  ob.it_co_credor = e.entnumcpfcnpj 
													 AND ob.ob NOT IN (
																	SELECT 
																		obf.obfnumero 
																	FROM 
																		contratos.faturacontrato fc
																	JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
																	WHERE 
																		fc.ftcstatus = 'A' AND
																		fc.ctrid =  $ctrid
																	)
						WHERE
							c.ctrstatus = 'A' AND
							c.ctrid = $ctrid
						group by ob.datatransacao	
						ORDER BY
							ob.datatransacao)
				) a
			ORDER BY
		        mesano";	
	$executado = $db->carregar($sql);
	if($executado)
		foreach($executado as $valor){
			$array2[$valor['data']]['executado']+=$valor['valorob'];
			$array2[$valor['data']]['mesano']=$valor['mesano'];
		}
	else 
		$array2 = array();
	$arrGrafico = array_merge_recursive($array1,$array2);

	ksort($arrGrafico);
	if ($arrGrafico) {
		$arrAxis = array ();
		$arrPeriodo = array ();
		$arrDados = array ();
		$arrAcumulado = array();
		$contador = 0;
		foreach ( $arrGrafico as $x=>$grafico ) :
			if (! in_array ( $x, $arrAxis )) {
				$arrAxis [$contador] =  is_array($grafico['mesano'])? $grafico['mesano'][0]:$grafico['mesano'];
			}
			if (! in_array ( $grafico ['mesano'], $arrPeriodo )) {
				$arrPeriodo [$contador] = is_array($grafico['mesano'])? $grafico['mesano'][0]:$grafico['mesano'];
				$arrDados [$contador]['planejado']  = $grafico ['planejado'];
				$arrDados [$contador]['executado']  = $grafico ['executado'];
			} else {
				$arrDados [$contador]['planejado']  = $grafico ['planejado'];
				$arrDados [$contador]['executado']  = $grafico ['executado'];
			}
			if($contador==0){
				$arrAcumulado[$contador]['planejado'] = $grafico ['planejado'];
				$arrAcumulado[$contador]['executado'] = $grafico ['executado'];
			}else{ 	
				$arrAcumulado[$contador]['planejado'] = $grafico ['planejado']+ (($arrAcumulado[$contador-1]['planejado'])?$arrAcumulado[$contador-1]['planejado']:0);
				$arrAcumulado[$contador]['executado'] = $grafico ['executado']+ (($arrAcumulado[$contador-1]['executado'])?$arrAcumulado[$contador-1]['executado']:0);
			}
			$contador++;
		endforeach;

		$contPeriodo = count ( $arrPeriodo );
		$contAxis = count ( $arrAxis );

		?>
		<div id="container" style="width: 700px; float: left"></div>
        <div id="graph2" style="width: 700px; float: left; height: 400px;"></div>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script src="../includes/Highcharts-3.0.0/js/highcharts.js"></script> 
	<script>
	$(function () {
		var chart = $('#container').highcharts(),
            chart2 = $('#graph2').highcharts();

	    $('#container').highcharts({
	        chart: {
	            zoomType: 'xy'
	        },
	        title: {
	            text: 'Gráfico de Desembolso'
	        },
	        subtitle: {
	            text: 'Acumulativo'
	        },
	        xAxis: {
                categories: [ <?= "'".implode($arrPeriodo,"','")."'"; ?>]
            },
	        yAxis: [ { // Secondary yAxis
	            gridLineWidth: 0,
	            title: {
	                text:'Desembolso Acumulativo',
	                style: {
	                    color: Highcharts.getOptions().colors[0]
	                }
	            },
	            labels: {
	                //format: '{value} mm',
	                style: {
	                    color: '#000000'
	                }
	            }

	        },{ // Primary yAxis
	        	gridLineWidth: 0,
	            labels: {
	                //format: 'Desembolso Mensal',
	                style: {
	                    color: '#000000'
	                }
	            },
	            title: {
	                text: 'Desembolso Acumulado ',
	                style: {
	                    color: 'white'
	                }
	            },
	            opposite: true

	        }],
	        tooltip: {
	            shared: true,
                formatter: function() {
                	 return "<b>"+this.x+"</b> <br><b>Planejado</b><br>Acum.: R$ "+Highcharts.numberFormat(this.points[0].y, 2, ',', '.')
                	 +"<br>Mensal: R$ "+Highcharts.numberFormat(this.points[1].y, 2, ',', '.')+"<br><b>Executado</b><br>Acum.: R$ "+Highcharts.numberFormat(this.points[2].y, 2, ',', '.')
                	 +"<br>Mensal: R$ "+Highcharts.numberFormat(this.points[3].y, 2, ',', '.');
                }
	        },
            plotOptions: {
            	series: {
                    dataLabels: {
                        //enabled: false,
                        formatter: function() {
                           // console.log(this.point);
                            return Highcharts.numberFormat(this.y, 2, ',', '.');
                        }
                    }
                },
                line: {
                    dataLabels: {
                        enabled: false
                    },
                    enableMouseTracking: true
                },
                column:{
                    dataLabels: {
                        enabled: false
                    },
                    enableMouseTracking: true
                }
            },
	        legend: {
	            layout: 'vertical',
	            align: 'left',
	            x: 100,
	            verticalAlign: 'top',
	            y: 0,
	            floating: true,
	            backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'
	        },

	        series: [{
	            name: 'Planejado Acumulado',
	            type: 'column',
	            yAxis: 0,
	            zIndex:1,
                color: 'green',
	            data: [<?php if($arrAcumulado) foreach($arrAcumulado as $a=>$dado)
			            {
			            	if($a!=0)
			            		echo ",";
							echo ($dado['planejado'])?$dado['planejado']:0;
			            }
	            	?>]
	        }, {
	            name: 'Executado Acumulado',
	            type: 'column',
	            //yAxis: 1,
	            zIndex:2,
                color: 'red',
	            data: [<?php if($arrAcumulado) foreach($arrAcumulado as $a=>$dado)
	            {
	            	if($a!=0)
	            		echo ",";
					echo ($dado['executado'])?$dado['executado']:0;
	            }
        	?>]
	        }]
	    });

        $('#graph2').highcharts({
            chart: {
                zoomType: 'xy'
            },
            title: {
                text: 'Gráfico de Desembolso'
            },
            subtitle: {
                text: 'Acumulativo'
            },
            xAxis: {
                categories: [ <?= "'".implode($arrPeriodo,"','")."'"; ?>]
            },
            yAxis: [ { // Secondary yAxis
                gridLineWidth: 0,
                title: {
                    text:'Desembolso Mensal',
                    style: {
                        color: Highcharts.getOptions().colors[0]
                    }
                },
                labels: {
                    //format: '{value} mm',
                    style: {
                        color: '#000000'
                    }
                }

            },{ // Primary yAxis
                gridLineWidth: 0,
                labels: {
                    //format: 'Desembolso Mensal',
                    style: {
                        color: '#000000'
                    }
                },
                title: {
                    text: 'Desembolso Acumulado ',
                    style: {
                        color: 'white'
                    }
                },
                opposite: true

            }],
            tooltip: {
                shared: true,
                formatter: function() {
                    return "<b>"+this.x+"</b> <br><b>Planejado</b><br>Acum.: R$ "+Highcharts.numberFormat(this.points[0].y, 2, ',', '.')
                        +"<br>Mensal: R$ "+Highcharts.numberFormat(this.points[1].y, 2, ',', '.')+"<br><b>Executado</b><br>Acum.: R$ "+Highcharts.numberFormat(this.points[2].y, 2, ',', '.')
                        +"<br>Mensal: R$ "+Highcharts.numberFormat(this.points[3].y, 2, ',', '.');
                }
            },
            plotOptions: {
                series: {
                    dataLabels: {
                        //enabled: false,
                        formatter: function() {
                            // console.log(this.point);
                            return Highcharts.numberFormat(this.y, 2, ',', '.');
                        }
                    }
                },
                line: {
                    dataLabels: {
                        enabled: false
                    },
                    enableMouseTracking: true
                },
                column:{
                    dataLabels: {
                        enabled: false
                    },
                    enableMouseTracking: true
                }
            },
            legend: {
                layout: 'vertical',
                align: 'left',
                x: 100,
                verticalAlign: 'top',
                y: 0,
                floating: true,
                backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'
            },

            series: [{
                name: 'Planejado Mensal',
                type: 'line',
                yAxis: 0,
                zIndex:3,
                data: [<?php if($arrDados) foreach($arrDados as $a=>$dado)
	            {
	            	if($a!=0)
	            		echo ",";
					echo ($dado['planejado'])?$dado['planejado']:0;
	            }
        	?>]
            },{
                name: 'Executado Mensal',
                type: 'line',
                yAxis: 0,
                zIndex:4,
                data: [<?php if($arrDados) foreach($arrDados as $a=>$dado)
	            {
	            	if($a!=0)
	            		echo ",";
					echo ($dado['executado'])?$dado['executado']:0;
	            }
        	?>]
            }]
        });

        $('#graph2').highcharts({
            chart: {
                zoomType: 'xy'
            },
            title: {
                text: 'Gráfico de Desembolso'
            },
            subtitle: {
                text: 'Mensal'
            },
            xAxis: {
                categories: [ <?= "'" . implode($arrPeriodo,"','") . "'"; ?>]
            },
            yAxis: [ { // Secondary yAxis
                gridLineWidth: 0,
                min: 0,
                title: {
                    text:'Desembolso Mensal',
                    style: {
                        color: Highcharts.getOptions().colors[0]
                    }
                },
                labels: {
                    //format: '{value} mm',
                    style: {
                        color: '#000000'
                    }
                }

            },{ // Primary yAxis
                gridLineWidth: 0,
                labels: {
                    //format: 'Desembolso Mensal',
                    style: {
                        color: '#000000'
                    }
                },
                title: {
                    text: 'Desembolso Acumulado ',
                    style: {
                        color: 'white'
                    }
                },
                opposite: true

            }],
            tooltip: {
                shared: true,
                formatter: function() {
                    return "<b>"+this.x+"</b> <br><b>Planejado</b><br>Acum.: R$ "+Highcharts.numberFormat(this.points[0].y, 2, ',', '.')
                        +"<br>Mensal: R$ "+Highcharts.numberFormat(this.points[1].y, 2, ',', '.')+"<br><b>Executado</b><br>Acum.: R$ "+Highcharts.numberFormat(this.points[2].y, 2, ',', '.')
                        +"<br>Mensal: R$ "+Highcharts.numberFormat(this.points[3].y, 2, ',', '.');
                }
            },
            plotOptions: {
                series: {
                    dataLabels: {
                        enabled: false,
                        formatter: function() {
                            return Highcharts.numberFormat(this.y, 2, ',', '.');
                        }
                    }
                },
                line: {
                    dataLabels: {
                        enabled: false
                    },
                    enableMouseTracking: true
                }
            },
            legend: {
                layout: 'vertical',
                align: 'left',
                x: 100,
                verticalAlign: 'top',
                y: 0,
                floating: true,
                backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'
            },

            series: [{
                name: 'Planejado Mensal',
                type: 'line',
                yAxis: 0,
                zIndex:3,
                color: 'red',
                data: [<?php if($arrDados) foreach($arrDados as $a=>$dado)
	            {
	            	if($a!=0)
	            		echo ",";
					echo ($dado['planejado'])?$dado['planejado']:0;
	            }
        	?>]
            },{
                name: 'Executado Mensal',
                type: 'line',
                yAxis: 0,
                zIndex:4,
                color: 'green',
                data: [<?php if($arrDados) foreach($arrDados as $a=>$dado)
	            {
	            	if($a!=0)
	            		echo ",";
					echo ($dado['executado'])?$dado['executado']:0;
	            }
        	?>]
            }]
        });
	});
	 </script>
	<?php }} ?>
	
