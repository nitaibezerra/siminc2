<?php
pde_verificaSessao();
/*
 * 
Function, salvar/editar
 */
function salva (){
	global $db,$pdeid;

	$sql = sprintf("DELETE from
			 	 pdeescola.fontedestinacaorecurso
			    WHERE
			     pdeid = %d",
			$pdeid
	);
	$db->executar($sql);

	
	
	//pega Esferas
	$sql = sprintf("select tieid, tiedescricao 
			from pdeescola.tipoesfera 
			order by tieid"
	);
	$Esferas = $db->carregar( $sql );
	$Esferas = $Esferas ? $Esferas : array();
	
	foreach ( $Esferas as $Esferas2 ) :
		
		//pega Fonte
		$sql = sprintf("select forid, fordescricao
				from pdeescola.fonterecurso 
				where tieid = %d
				order by forid",
				$Esferas2['tieid']
		);
		$Fontes = $db->carregar( $sql );
		$Fontes = $Fontes ? $Fontes : array();
		
		foreach ( $Fontes as $Fontes2 ) :			
			
			$valor2 = $_POST['v2_'.$Esferas2["tieid"].'_'.$Fontes2["forid"]];
			$valor2 = str_replace('.','',$valor2);
			$valor2 = str_replace(',','.',$valor2);
			$valor2 = str_replace(' ','',$valor2);
			$valor2 = (float)$valor2;

			$valor3 = $_POST['v3_'.$Esferas2["tieid"].'_'.$Fontes2["forid"]];
			$valor3 = str_replace('.','',$valor3);
			$valor3 = str_replace(',','.',$valor3);
			$valor3 = str_replace(' ','',$valor3);
			$valor3 = (float)$valor3;
			
			$valor4 = $_POST['v4_'.$Esferas2["tieid"].'_'.$Fontes2["forid"]];
			$valor4 = str_replace('.','',$valor4);
			$valor4 = str_replace(',','.',$valor4);
			$valor4 = str_replace(' ','',$valor4);
			$valor4 = (float)$valor4;
			
			$valor5 = $_POST['v5_'.$Esferas2["tieid"].'_'.$Fontes2["forid"]];
			$valor5 = str_replace('.','',$valor5);
			$valor5 = str_replace(',','.',$valor5);
			$valor5 = str_replace(' ','',$valor5);
			$valor5 = (float)$valor5;
			
			$sql = sprintf("INSERT INTO pdeescola.fontedestinacaorecurso
					    (
					     pdeid,
					     forid,
					     fdraperfeicoamentopessoal,
					     fdrmaterialinstitucional,
					     fdrmanutencao,
					     fdroutros,
					     fdranoreferencia,
					     fdrdataatualizacao
					    )VALUES(
					     %d,
					     %d,
						 %s,
						 %s,
						 %s,
						 %s,
						 %d,
						 %s					    
						)",
					$pdeid,
					$Fontes2['forid'],
					$valor2 ? $valor2 : 'null',
					$valor3 ? $valor3 : 'null',
					$valor4 ? $valor4 : 'null',
					$valor5 ? $valor5 : 'null',
					ANO_EXERCICIO_PDE_ESCOLA,
					'now()'
			);
			//echo '<br>'.$sql;
			$db->executar($sql);
			
		endforeach;
		
	endforeach;
	//die();
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p15' AND
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
											 	ppritem = 'p15'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p15','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$fscid = $_POST['fscid'] ? $_POST['fscid'] : $_SESSION['fscid'];
$pdeid = $_POST['pdeid'] ? $_POST['pdeid'] : $_SESSION['pdeid'];

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

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva();
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href = \'?modulo=principal/instrumento1/p11&acao=A\';
		 </script>');
endif;
*/

/*
 * Carrega dados
 */
$sql = "SELECT
		 *
		FROM
		 pdeescola.formaselecaodiretor
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1';
monta_titulo($titulo_modulo, '');
?>
<?=cabecalhoPDE(); ?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?=subCabecalho('15 - Fontes e destinação dos recursos utilizados pela Escola.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" id="tabela">
	<tr bgcolor="#dfdfdf">
		<td align="center">
			<b>Tipo Esfera</b>
		</td>
		<td align="center">
			<b>Fonte</b>
		</td>
		<td align="center">
			<b>Aperfeiçoamento de pessoal</b>
		</td>
		<td align="center">
			<b>Materiais Instrucionais</b>
		</td>
		<td align="center">
			<b>Manutenção</b>
		</td>
		<td align="center">
			<b>Outros</b>
		</td>
		<td align="center">
			<b>Total (R$)</b>
		</td>
	</tr>
	<?

		//pega Esferas
		$sql = sprintf("select tieid, tiedescricao 
				from pdeescola.tipoesfera 
				order by tieid"
		);
		$Esferas = $db->carregar( $sql );
		$Esferas = $Esferas ? $Esferas : array();
		
		$cor = '';
		
 
		for( $i = 0; $i < count($Esferas); $i++)
		{
			
			$query = "select forid, fordescricao
						from pdeescola.fonterecurso 
						where tieid = '{$Esferas[$i]['tieid']}'
						order by forid";
											
			$rspega = $db->carregar( $query );
 
			for( $k = 0; $k <count( $rspega ); $k++)
			{
			
				$query2 ="select fdraperfeicoamentopessoal as soma_geral2, 
										fdrmaterialinstitucional as soma_geral3, 
       									fdrmanutencao as soma_geral4, 
       									fdroutros as soma_geral5
       									from pdeescola.fontedestinacaorecurso
       							where pdeid = $pdeid
								and forid= '{$rspega[$k]['forid']}'";
 
				$fontessoma = $db->carregar( $query2 );
 
				for( $j = 0; $j< count($fontessoma); $j++ )
				{
					if( count( $fontessoma) > 0)
					{	
						
						$soma_geral2 += $fontessoma[$j]['soma_geral2'];
						$soma_geral3 += $fontessoma[$j]['soma_geral3'];
						$soma_geral4 += $fontessoma[$j]['soma_geral4'];
						$soma_geral5 += $fontessoma[$j]['soma_geral5']; 
					}
				}
				 
			} 
		}
		 
		foreach ( $Esferas as $Esferas2 ) :
			$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;
 
			$sql = sprintf("select forid, fordescricao
					from pdeescola.fonterecurso 
					where tieid = %d
					order by forid",
					$Esferas2['tieid']
			);
			$Fontes = $db->carregar( $sql );
			$Fontes = $Fontes ? $Fontes : array();
			
			
			$ct=1;
			$total1 = '';
			$total2 = '';
			$total3 = '';
			$total4 = '';
			$total5 = '';
			foreach ( $Fontes as $Fontes2 ) :	
				
						$sql = sprintf("select fdraperfeicoamentopessoal as valor2, 
										fdrmaterialinstitucional as valor3, 
       									fdrmanutencao as valor4, 
       									fdroutros as valor5
       									from pdeescola.fontedestinacaorecurso
       							where pdeid=%d
								and forid=%d
								 GROUP BY fdraperfeicoamentopessoal, fdrmaterialinstitucional, fdrmanutencao, fdroutros
								",
							$pdeid,
							$Fontes2['forid']
						);
						$dados = (array) $db->pegaLinha($sql);
						
						
						$valor1 = '';
						$valor2 = '';
						$valor3 = '';
						$valor4 = '';
						$valor5 = '';

						 
						if($dados['valor2']){
							$valor2 = simec_number_format($dados['valor2'],2,',','.');
							$total2 = $total2 + $dados['valor2'];
							$total_geral2 = $total_geral2 + $dados['valor2'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
						
						   
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial2 = ( $valor2 / $totparcial ) * 100;
							}							
							$valor1 = $valor1 + $dados['valor2'];
								  
							
						}
						if($dados['valor3']){
							$valor3 = simec_number_format($dados['valor3'],2,',','.');
							$total3 = $total3 + $dados['valor3'];
							$total_geral3 = $total_geral3 + $dados['valor3'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
						
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial3 = ( $valor3 / $totparcial ) * 100;	
							}
							$valor1 = $valor1 + $dados['valor3'];
							
							 
						}
						if($dados['valor4']){
							$valor4 = simec_number_format($dados['valor4'],2,',','.');
							$total4 = $total4 + $dados['valor4'];
							$total_geral4 = $total_geral4 + $dados['valor4'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial4 = ( $valor4 / $totparcial ) * 100;	
							}
							$valor1 = $valor1 + $dados['valor4'];
							 
						}
						if($dados['valor5']){
							$valor5 = simec_number_format($dados['valor5'],2,',','.');
							$total5 = $total5 + $dados['valor5'];
							$total_geral5 = $total_geral5 + $dados['valor5'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial5 = ( $valor5 / $totparcial ) * 100;	
							}
							$valor1 = $valor1 + $dados['valor5'];
							 
						} 
						if($valor1){
							$total1 = $total1 + $valor1;
							$total_geral1 = $total_geral1 + $valor1;
							$valor1 = simec_number_format($valor1,2,',','.');
							
							$soma_total = ( $soma_geral2 + $soma_geral3 + $soma_geral4 + $soma_geral5 );
							
							if((  $soma_total != 0 ) && ( $soma_total != ''))
							{
								$geral_parcial1 = ($total1 * 100)/ $soma_total;
							}
							
						}
						//echo $soma_total;
//				echo $totparcial;    
//				echo '<br>';
//				 
				
				?>
				<tr bgcolor="<?= $cor ?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';"><!-- onmouseover="this.style.backgroundColor='#ffffcc';"> -->
					<?if($cod_esfera != $Esferas2['tieid']){?>
						<td align="left" rowspan="<?=count($Fontes)+2?>" id="tdr_<?=$Esferas2['tieid']?>">
							<?= $Esferas2['tiedescricao'];?>
						</td>
					<?}?>
					<td align="center" >
						<?= strtoupper($Fontes2['fordescricao']); ?>
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor2?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v2_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual();" onblur="calcula('v2_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); verificaCampo();">
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v3_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v3_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor3?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v3_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); "onblur="calcula('v3_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); verificaCampo();">
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v4_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v4_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor4?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v4_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); "onblur="calcula('v4_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); verificaCampo();">
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v5_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v5_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor5?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v5_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); " onblur="calcula('v5_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); verificaCampo();">
					</td>
					<td align="center" >
						<input disabled class='CampoEstilo' type='text' name='v1_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v1_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor1?>' style='text-align:right;' >
					</td>
				</tr>
				<?

				if($ct == count($Fontes)){
				?>
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt2_<?=$Esferas2['tieid']?>' id='vt2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt3_<?=$Esferas2['tieid']?>' id='vt3_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total3,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt4_<?=$Esferas2['tieid']?>' id='vt4_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total4,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt5_<?=$Esferas2['tieid']?>' id='vt5_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total5,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt1_<?=$Esferas2['tieid']?>' id='vt1_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total1,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>%Total Geral:</b></td>
						<?php
						/*
						echo $total2.'<br>';
						echo $total_geral2.'<br>';
						echo $soma_geral2.'<br>';
						echo("<br>");
						*/ 
						if( $totparcial == '' )
						{
							$geral_parcial2 = 0; 
							$geral_parcial3 = 0; 
							$geral_parcial4 = 0; 
							$geral_parcial5 = 0;
						}
						?>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg2_<?=$Esferas2['tieid']?>' id='vtg2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial2,1,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg3_<?=$Esferas2['tieid']?>' id='vtg3_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial3,1,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg4_<?=$Esferas2['tieid']?>' id='vtg4_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial4,1,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg5_<?=$Esferas2['tieid']?>' id='vtg5_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial5,1,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg1_<?=$Esferas2['tieid']?>' id='vtg1_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial1,1,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<!-- 
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>% SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp1_<?=$Esferas2['tieid']?>' id='vp1_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp2_<?=$Esferas2['tieid']?>' id='vp2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp3_<?=$Esferas2['tieid']?>' id='vp3_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp4_<?=$Esferas2['tieid']?>' id='vp4_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp5_<?=$Esferas2['tieid']?>' id='vp5_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					 -->
				<?
				}

				$ct=$ct+1;
				$cod_esfera = $Esferas2['tieid'];

			endforeach;
			
			//echo $geral_parcial2.'<br>';
			
			$g2 += $geral_parcial2;
			$g3 += $geral_parcial3;
			$g4 += $geral_parcial4;
			$g5 += $geral_parcial5;
			
			
			
			if( $total_geral1 != 0)
			{
			  	$vttg2 = ($total_geral2 * 100)/ $total_geral1; 
				$vttg3 = ($total_geral3 * 100)/ $total_geral1;
				$vttg4 = ($total_geral4 * 100)/ $total_geral1;
				$vttg5 = ($total_geral5 * 100)/ $total_geral1;
			}
/*			
			if( $total1 != 0)
			{
			  	$vttg2 = ($total2 / $total1) * 100; 
				$vttg3 = ($total3 / $total1) * 100;
				$vttg4 = ($total4 / $total1) * 100;
				$vttg5 = ($total5 / $total1) * 100;
			}
*/
		 
		//die();
		endforeach;
		
	?>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot2' id='tot2' size='17' maxlength='14' value='<?=simec_number_format($total_geral2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot3' id='tot3' size='17' maxlength='14' value='<?=simec_number_format($total_geral3,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot4' id='tot4' size='17' maxlength='14' value='<?=simec_number_format($total_geral4,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot5' id='tot5' size='17' maxlength='14' value='<?=simec_number_format($total_geral5,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot1' id='tot1' size='17' maxlength='14' value='<?=simec_number_format($total_geral1,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>%TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg2' id='totg2' size='17' maxlength='14' value='<?=simec_number_format($vttg2,0,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg3' id='totg3' size='17' maxlength='14' value='<?=simec_number_format($vttg3,0,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg4' id='totg4' size='17' maxlength='14' value='<?=simec_number_format($vttg4,0,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg5' id='totg5' size='17' maxlength='14' value='<?=simec_number_format($vttg5,0,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg1' id='totg1' size='17' maxlength='14' value='<?=simec_number_format('100',0,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<!-- 
	<tr bgcolor='lightblue'>
		<td colspan=2 align=center ><b>% TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per1' id='per1' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per2' id='per2' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per3' id='per3' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per4' id='per4' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per5' id='per5' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	-->
</table>
<table width="95%" align="center">
  <?php echo $cForm->montarbuttons("validaForm()");?>	
</table>
<script type="text/javascript">
d = document;
function validaForm(){

	d.formulario.submit();
	return true;	
} 

//tratando campos para não permitir que o usuário arraste e cole texto.
function somenteNum(campo, idEsfera, idFonte)
{
	var strCampo = campo+idEsfera+'_'+idFonte;
	var valorCampo = document.getElementById(""+strCampo+"").value; 
	//alert(strCampo);
	
 	if (!isNaN(valorCampo)) {
 		//alert('vai mudar');
 	    valorCampo.value = 0;
    return false;
    }
}   

calculaPercentual();
 
//calcula('v2_', '1', '1');
//calcula('v3_', '1', '1');
//calcula('v4_', '1', '1');
//calcula('v5_', '1', '1'); 
function calcula(campo, idEsfera, idFonte)
{
//alert(campo);
//alert(idEsfera);
//alert(idFonte);
//return false;

//tratando campos para não permitir que o usuário arraste e cole texto.
	var strCampo = campo+idEsfera+'_'+idFonte;
	var valorCampo = document.getElementById(""+strCampo+"").value; 
	//alert(strCampo);
	
 	if (!isNaN(valorCampo)) {
 		//alert('vai mudar');
 	    valorCampo.value = 0;
    return false;
    }
    
	idEsferaLen = idEsfera.length + 3;
	form = document.getElementsByTagName("input");
	//alert(idEsferaLen);
	//alert(campo);
	
	var valor = 0;
	var valortotal = 0;
	var valortotalgeral = 0;
	var valortotalgeralparc = 0;
	var v1total = 0;
	var v1totalgeral = 0;
	
	//calcula linha
	somafont2 = 0;
	somafont3 = 0;
	somafont4 = 0;
	somafont5 = 0;
	for(i=0; i<form.length; i++) {
		//calcula soma fonte
		tam = form[i].id.length;
		font = form[i].id.substr(tam - idFonte.length,tam);
		if(font == idFonte) {
			if( form[i].id.substr(0,3) == 'v2_') {
				somafont2 = document.getElementById(form[i].id).value;
				if(!somafont2) somafont2 = 0; 
				somafont2 = somafont2.toString().replace(".","");
				somafont2 = somafont2.toString().replace(".","");
				somafont2 = somafont2.toString().replace(",",".");
				
							
			}
			if( form[i].id.substr(0,3) == 'v3_') {
				somafont3 = document.getElementById(form[i].id).value;
				if(!somafont3) somafont3 = 0; 
				somafont3 = somafont3.toString().replace(".","");
				somafont3 = somafont3.toString().replace(".","");
				somafont3 = somafont3.toString().replace(",",".");
				
				
			}
			if( form[i].id.substr(0,3) == 'v4_') {
				somafont4 = document.getElementById(form[i].id).value;
				if(!somafont4) somafont4 = 0; 
				somafont4 = somafont4.toString().replace(".","");
				somafont4 = somafont4.toString().replace(".","");
				somafont4 = somafont4.toString().replace(",",".");
				
				
			}
			if( form[i].id.substr(0,3) == 'v5_') {
				somafont5 = document.getElementById(form[i].id).value;
				if(!somafont5) somafont5 = 0; 
				somafont5 = somafont5.toString().replace(".","");
				somafont5 = somafont5.toString().replace(".","");
				somafont5 = somafont5.toString().replace(",",".");
				
				
			}
			
		}
	}
	
	//total linha
	total_linha = parseFloat(somafont2) + parseFloat(somafont3) + parseFloat(somafont4) + parseFloat(somafont5);  
	document.getElementById("v1_"+idEsfera+"_"+idFonte).value = float2moeda(total_linha);
	
	
	//calcula coluna
	for(i=0; i<form.length; i++) {
		//subtotal
		if( form[i].id.substr(0,idEsferaLen) == campo+idEsfera) {
			valor = document.getElementById(form[i].id).value;
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(",",".");
			if(!valor) valor = 0;
		
			valortotal = valortotal + parseFloat(valor);
		
				var subgeral = document.getElementById("vt1_"+idEsfera).value;
			 	subgeral = subgeral.toString().replace(".","");
			 	subgeral = subgeral.toString().replace(",",".");  
				//alert( 'valor total: (' + valortotal +' ) dividido por subgeral2( '+subgeral2+' )');
			 	 	
				var numCerto = Math.round(Number( valortotal / subgeral ) * 100);

			
			//aki começa a fazer o calculo subtoal geral.
			if( form[i].id.substr(0,3) == 'v2_') {
			//begin parcial test
				var totgeral2 = document.getElementById('tot2').value;
				var n_100_2 = Number(valortotal * 100);
				var divisor2 = totgeral2;
				var subtot2 = document.getElementById('vt2_'+idEsfera).value;
				divisor2 = divisor2.toString().replace(".","");
				divisor2 = divisor2.toString().replace(",",".");
		 		var tot2 = Math.round(n_100_2/divisor2);
		 		 
		 		//bgn
//		 		 
//				var subgeral2 = document.getElementById("vt1_"+idEsfera).value;
//			 	subgeral2 = subgeral2.toString().replace(".","");
//			 	subgeral2 = subgeral2.toString().replace(",",".");  
//				//alert( 'valor total: (' + valortotal +' ) dividido por subgeral2( '+subgeral2+' )');
//			 	
			 	
				//	var numCerto2 = Math.round(Number( valortotal / subgeral2 ) * 100);		 		
		 				 		//end
		 		
		 		document.getElementById('vtg2_'+idEsfera).value = numCerto+'%';
		 		  
			//end parcial test	
			}
			if( form[i].id.substr(0,3) == 'v3_') {
			//begin parcial test
				var totgeral3 = document.getElementById('tot3').value;
				var n_100_3 = Number(somafont3 * 100);
				var divisor3 = totgeral3;
				divisor3 = divisor3.toString().replace(".","");
				divisor3 = divisor3.toString().replace(",",".");
		 		//var tot3 = Math.round(n_100_3/divisor3);
				var tot3 = Number(n_100_3/divisor3);
				tot3 = tot3.toFixed(1);
						
				//bgn
		 		 
//				var subgeral3 = document.getElementById("vt1_"+idEsfera).value;
//			 	subgeral3 = subgeral3.toString().replace(".","");
//			 	subgeral3 = subgeral3.toString().replace(",",".");  
//				
//				var numCerto3 = Math.round(Number( valortotal / subgeral3 ) * 100);		 		
//		 		
		 		//end
		 		 
		 		document.getElementById('vtg3_'+idEsfera).value = numCerto+'%';
			//end parcial test		
			}
			if( form[i].id.substr(0,3) == 'v4_') {	
				//begin parcial test
				var totgeral4 = document.getElementById('tot4').value;
				var n_100_4 = Number(somafont4 * 100);
				var divisor4 = totgeral4;
				divisor4 = divisor4.toString().replace(".","");
				divisor4 = divisor4.toString().replace(",",".");
		 		//var tot4 = Math.round(n_100_4/divisor4);
		 		var tot4 = Number(n_100_4/divisor4);
		 		tot4 = tot4.toFixed(1);
		 		
				//bgn
		 		 
//				var subgeral4 = document.getElementById("vt1_"+idEsfera).value;
//			 	subgeral4 = subgeral4.toString().replace(".","");
//			 	subgeral4 = subgeral4.toString().replace(",",".");  
//				
//				var numCerto4 = Math.round(Number( valortotal / subgeral4 ) * 100);		 		
//		 		
		 		//end 
		 		
		 		document.getElementById('vtg4_'+idEsfera).value = numCerto+'%';
			//end parcial test		
			}
			if( form[i].id.substr(0,3) == 'v5_') {
			//begin parcial test
				var totgeral5 = document.getElementById('tot5').value;
				var n_100_5 = Number(somafont5 * 100);
				var divisor5 = totgeral5;
				divisor5 = divisor5.toString().replace(".","");
				divisor5 = divisor5.toString().replace(",",".");
		 		//var tot5 = Math.round(n_100_5/divisor5);
		 		var tot5 = Number(n_100_5/divisor5);
		 		tot5 = tot5.toFixed(1);
		 		
				//bgn
		 		 
//				var subgeral5 = document.getElementById("vt1_"+idEsfera).value;
//			 	subgeral5 = subgeral5.toString().replace(".","");
//			 	subgeral5 = subgeral5.toString().replace(",",".");  
//				
//				var numCerto5 = Math.round(Number( valortotal / subgeral5 ) * 100);	 
//		 		//end 
		 		document.getElementById('vtg5_'+idEsfera).value = numCerto+'%';
			//end parcial test		
			}
		}
		
		//totalgeral/PARCIALNEW
		if( form[i].id.substr(0,3) == campo) {
			valor = document.getElementById(form[i].id).value;
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(",",".");
			if(!valor) valor = 0;
			valortotalgeralparc = valortotalgeral + parseFloat(valor);
		}
		
		
		//totalgeral
		if( form[i].id.substr(0,3) == campo) {
			valor = document.getElementById(form[i].id).value;
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(",",".");
			if(!valor) valor = 0;
			valortotalgeral = valortotalgeral + parseFloat(valor);
		}
		//coluna total (subtotal)
		if( form[i].id.substr(0,idEsferaLen) == 'v1_'+idEsfera) {
			v1 = document.getElementById(form[i].id).value;
			v1 = v1.toString().replace(".","");
			v1 = v1.toString().replace(".","");
			v1 = v1.toString().replace(",",".");
			if(!v1) v1 = 0;
			v1total = v1total + parseFloat(v1);
			document.getElementById("vt1_"+idEsfera).value = float2moeda(v1total);
		}
		//coluna total (total geral)
		if( form[i].id.substr(0,4) == 'vt1_') {
			//alert(form[i].id);
			v2 = document.getElementById(form[i].id).value;
			v2 = v2.toString().replace(".","");
			v2 = v2.toString().replace(".","");
			v2 = v2.toString().replace(",",".");
			if(!v2) v2 = 0;
			//alert(v2);
			 
			v1totalgeral = v1totalgeral + parseFloat(v2);
			
			    
			document.getElementById("tot1").value = float2moeda(v1totalgeral);
		}

	}
	if(campo == 'v2_'){
	
	
		var vtt2 = totgeral2;
		vtt2 = vtt2.toString().replace(".","");
		vtt2 = vtt2.toString().replace(",",".");
		
		var vtot2 = v1totalgeral;
		
		var vtt2_x100 = Number( vtt2 * 100 ); 
		var vttg2   = Math.round(Number( vtt2_x100 ) / Number( vtot2));
		//var vttg2 = Number( Number( vtt2_x100 ) / Number( vtot2) );
		//vttg2 = vttg2.toFixed(1);
	 
		//subtotal
		document.getElementById("vt2_"+idEsfera).value = float2moeda(valortotal);
		//totalgeral
		document.getElementById("tot2").value = float2moeda(valortotalgeral);
		
 	     //%totalgeral 
    	 document.getElementById("totg2").value = vttg2+'%';
		
	}
	if(campo == 'v3_'){
	
		var vtt3 = totgeral3;
		vtt3 = vtt3.toString().replace(".","");
		vtt3 = vtt3.toString().replace(",",".");
		
		var vtot3 = v1totalgeral;
		
		var vtt3_x100 = Number( vtt3 * 100 ); 
		var vttg3   = Math.round(Number( vtt3_x100 ) / Number( vtot3));
		//var vttg3 = Number( Number( vtt3_x100 ) / Number( vtot3) );
		//vttg3 = vttg3.toFixed(1);
		 
		//subtotal
		document.getElementById("vt3_"+idEsfera).value = float2moeda(valortotal);
		//totalgeral
		document.getElementById("tot3").value = float2moeda(valortotalgeral);
 		//%totalgeral 
 		document.getElementById("totg3").value = vttg3+'%';
	}
	if(campo == 'v4_'){
	
		var vtt4 = totgeral4;
		vtt4 = vtt4.toString().replace(".","");
		vtt4 = vtt4.toString().replace(",",".");
		
		var vtot4 = v1totalgeral;
		
		var vtt4_x100 = Number( vtt4 * 100 ); 
		var vttg4   = Math.round(Number( vtt4_x100 ) / Number( vtot4));
		//var vttg4 = Number( Number( vtt4_x100 ) / Number( vtot4) );
		//vttg4 = vttg4.toFixed(1);
		
		//subtotal
		document.getElementById("vt4_"+idEsfera).value = float2moeda(valortotal);
		//totalgeral
		document.getElementById("tot4").value = float2moeda(valortotalgeral);
 		//%totalgeral 
 		document.getElementById("totg4").value = vttg4+'%';
	}
	if(campo == 'v5_'){
	
		var vtt5 = totgeral5;
		vtt5 = vtt5.toString().replace(".","");
		vtt5 = vtt5.toString().replace(",",".");
		
		var vtot5 = v1totalgeral;
		
		var vtt5_x100 = Number( vtt5 * 100 ); 
		var vttg5   = Math.round(Number( vtt5_x100 ) / Number( vtot5));
		//var vttg5 = Number( Number( vtt5_x100 ) / Number( vtot5) );
		//vttg5 = vttg5.toFixed(1);
		
		//subtotal
		document.getElementById("vt5_"+idEsfera).value = float2moeda(valortotal);
		//totalgeral
		document.getElementById("tot5").value = float2moeda(valortotalgeral);
 		//%totalgeral 
 		document.getElementById("totg5").value = vttg5+'%';
	}
				 
	
}
	
function calculaPercentual()
{
	var subgeral;
	var subtot;
	var subpercentual;
	
		for( var i = 1; i <= 5; i++ )
		{
			for( var j = 1; j<= 4; j++ )
			{ 
				subtot = document.getElementById('vt'+i+'_'+j+'').value;
				subtot = subtot.toString().replace(".","");
			 	subtot = subtot.toString().replace(",",".");  
			 	
				subgeral = document.getElementById('vt1_'+j+'').value;
				subgeral = subgeral.toString().replace(".","");
			 	subgeral = subgeral.toString().replace(",",".");  
				//subpercentual = Math.round(Number( subtot / subgeral ) * 100);
				subpercentual = Number(Number( subtot / subgeral ) * 100);
				subpercentual = subpercentual.toFixed(1);
				
				if(isNaN(subpercentual))
				{
					subpercentual = 0;
				}
	       		{ 
					document.getElementById('vtg'+i+'_'+j).value = subpercentual+'%';
				}
			}
		}				 

}

function float2moeda(num) {
   x = 0;
   if(num<0) {
      num = Math.abs(num);
      x = 1;
   }
   if(isNaN(num)) num = "0";
      cents = Math.floor((num*100+0.5)%100);
   num = Math.floor((num*100+0.5)/100).toString();
   if(cents < 10) cents = "0" + cents;
      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
         num = num.substring(0,num.length-(4*i+3))+'.'
               +num.substring(num.length-(4*i+3));
    ret = num + ',' + cents;
    if (x == 1) ret = ' - ' + ret;
    return ret;
}

//function verificaCampo()
//{
//	var subgeral;
//	var subtot;
//	var subpercentual;
//	
//		for( var i = 1; i <= 5; i++ )
//		{
//			for( var j = 1; j<= 4; j++ )
//			{ 
//				var campo =	document.getElementById('vtg'+i+'_'+j).value.length;
//			
//				if( campo > 500 )
//				{
//					alert("Você excedeu o nº máximo de caracteres (500) do campo 'Evidência");
//					return false;
//				}
//	
//			}
//		}				 
//
//}

</script>	