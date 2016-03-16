<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

//carregando valor do paf no banco.
	$sql = "SELECT
	DISTINCT
	coalesce(pde.pdevlrplanocapital,0) as pdevlrplanocapital,
	coalesce(pde.pdevlrplanocusteio,0) as pdevlrplanocusteio
	FROM
	entidade.entidade ent
	INNER JOIN entidade.funcaoentidade as fe ON fe.entid = ent.entid
	INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
	INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid
	INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
	INNER JOIN territorios.estado est ON est.estuf = mun.estuf
	INNER JOIN pdeescola.pdeescola pde ON ent.entid = pde.entid
	WHERE
	fe.funid = 3 and
	ent.tpcid IN (1,3) AND
	ent.entid IN ('{$_SESSION['entid']}')";

	$dados = $db->carregar($sql);

	$percent_capital = $dados[0]['pdevlrplanocapital'];
	$percent_custeio = $dados[0]['pdevlrplanocusteio'];
	$paf = $percent_capital + $percent_custeio;
	
/*
 * 
Function, salvar/editar
 */

			
function salva (){
	global $db,$pdeid;

	$sql = sprintf("DELETE from
			 	 pdeescola.previsaorecursosescola
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

			$sql = sprintf("INSERT INTO pdeescola.previsaorecursosescola
					    (
					     pdeid,
					     forid,
					     prevalortotal,
					     preanoreferencia,
					     predataatualizacao
					    )VALUES(
					     (%d),
					     (%d),
						 (%s),
						 (%d),
						  %s					    
						)",
					$pdeid,
					$Fontes2['forid'],
					trim(str_replace(' ', '', $valor2)) ? trim(str_replace(' ', '', $valor2)): 'null',
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
							  	trim(epfnomearquivo) = 'p16' AND
							  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
	
	$existe = $db->pegaUm("SELECT
							pepid
						 FROM
						 	pdeescola.pdeepf
						 WHERE
						 	pdeid = ".$pdeid." AND
						 	epfid = ".$epfid);
	
	if($existe == NULL) {
		$sql = "INSERT INTO pdeescola.pdeepf (pdeid,epfid) VALUES (".$pdeid.",".$epfid.")";
		$db->executar($sql);
	}
		
	
	$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p16'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p16','{$_SESSION['exercicio_atual']}')";
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
			location.href = \'?modulo=principal/instrumento1/p12&acao=A\';
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
<?=subCabecalho('16 - Previsão de recursos da escola para o ano corrente, segundo fontes.'); ?>	
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
  
		//workaround 
		for( $i = 0; $i < count($Esferas); $i++)
		{ 
			$query = "select forid, fordescricao
					from pdeescola.fonterecurso 
					where tieid = '{$Esferas[$i]['tieid']}'
					order by forid";
							
			$rspega = $db->carregar( $query );
			 
			for( $k = 0; $k <count( $rspega ); $k++)
			{ 
				if( $query['forid'] != 5){
				$query2 ="select prevalortotal as valor2 
								from pdeescola.previsaorecursosescola 
								where pdeid= '$pdeid'
								and forid = '{$rspega[$k]['forid']}'";
			}else{								
				
				$query2 ="select prevalortotal as valor2 
								from pdeescola.previsaorecursosescola 
								where pdeid= '$pdeid'
								and forid = '{$rspega[1]['forid']}'";
			}
				$fontessoma = $db->carregar( $query2 );
				 
				for( $j = 0; $j< count($fontessoma); $j++ )
				{
					if( count( $fontessoma) > 0)
					{	 
						$soma_geral += $fontessoma[$j]['valor2']; 
					}
				} 
			} 
		} 		
		//end workaround	
		$soma_geral = ($soma_geral + $paf);
		
		foreach ( $Esferas as $Esferas2 ) :
			
			$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;

			
			//pega Fonte
			$sql = sprintf("select forid, fordescricao
					from pdeescola.fonterecurso 
					where tieid = %d
					order by forid",
					$Esferas2['tieid']
			);
			$Fontes = $db->carregar( $sql );
			$Fontes = $Fontes ? $Fontes : array();

			$ct=1;
			$total2 = 0;
			foreach ( $Fontes as $Fontes2 ) :	
				
						$sql = sprintf("select prevalortotal as valor2 
								from pdeescola.previsaorecursosescola 
								where pdeid=%d
								and forid=%d
								",
								$pdeid,
								$Fontes2['forid']
								);
								
						$dados = (array) $db->pegaLinha($sql);
						
						$valor2 = 0;
						$valorSoma = 0;
						
						if( $Fontes2['forid'] == "5")
						{
							$valor2   = number_format($paf,2,',','.');
							$valorSoma = (integer) $paf;
							$disable = "disabled = true";
						}
						else {	
							if($dados['valor2'])
							{
								$valor2 = number_format($dados['valor2'],2,',','.');
								$valorSoma = $dados['valor2'];	
						    }
						    $disable = "";  
						}
						
						$total2 += $valorSoma;
						$total_geral2 += $valorSoma;
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
						<input class='CampoEstilo' type='text' name='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14'<?=$disable ?> value='<?=$valor2?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v2_','<?=$Esferas2['tieid']?>');" onblur="calcula('v2_','<?=$Esferas2['tieid']?>');" />
					</td>
				</tr>
				
				<?

				if($ct == count($Fontes)){
				?>
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt2_<?=$Esferas2['tieid']?>' id='vt2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=number_format($total2, 2, ',', '.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<tr bgcolor='#ffffcc'>
						<?php
						$total_porcent = 0;
						
						if( $soma_geral > 0 ) {
							$total_porcent = (($total2 * 100) / $soma_geral);
						}
						?>
						<td align=right ><b>%Total Geral:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg2_<?=$Esferas2['tieid']?>' id='vtg2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=number_format($total_porcent,2,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<!-- 
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>% SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp2_<?=$Esferas2['tieid']?>' id='vp2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					 -->
				<?
				}
	
				$ct=$ct+1;
				$cod_esfera = $Esferas2['tieid'];

			endforeach;
			
			if( $soma_geral > 0 )
				$vttg = ($total2 * 100) / $soma_geral;

		endforeach;

	?>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot2' id='tot2' size='17' maxlength='14' value='<?=number_format($total_geral2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>%TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg2' id='totg2' size='17' maxlength='14' value='<?=number_format('100',0,',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<!-- 
	<tr bgcolor='lightblue'>
		<td colspan=2 align=center ><b>% TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per2' id='per2' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
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

function calcula(campo, idEsfera)
{
	idEsferaLen = idEsfera.length + 3;
	form = document.getElementsByTagName("input");
	//alert(idEsferaLen);
	//alert(campo);
	
	var valor = 0;
	var valortotal = 0;
	var valortotalgeral = 0;
	
	for(i=0; i<form.length; i++) {
		//subtotal
		if( form[i].id.substr(0,idEsferaLen) == campo+idEsfera) {
			valor = document.getElementById(form[i].id).value;
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(".","");
			valor = valor.toString().replace(",",".");
			if(!valor) valor = 0;
			valortotal = valortotal + parseFloat(valor);
			
			
			
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
		
		
	}
	
	
	if(campo == 'v2_'){
		//subtotal
		document.getElementById("vt2_"+idEsfera).value = float2moeda(valortotal);
		//totalgeral
		document.getElementById("tot2").value = float2moeda(valortotalgeral);
		
		var tot_parcial = Number( (valortotal * 100 ) / valortotalgeral);
		//document.getElementById("vtg2_"+idEsfera).value = float2moeda(tot_parcial);
		document.getElementById("vtg2_"+idEsfera).value = tot_parcial.toFixed(2)+'%';
		
		//document.getElementById("vtg2_"+idEsfera).value = Number(tot_parcial)+'%';
		 
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

</script>	