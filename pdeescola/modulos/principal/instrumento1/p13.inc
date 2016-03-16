<?php
pde_verificaSessao();
/*----- Recuperando $docid para usar na função pegarEstadoAtual ------*/ 
$docid  = pegarDocid( $_SESSION['entid'] );
$estado = pegarEstadoAtual( $docid );
$estadoPerm = (($estado == 76) || ($estado == 37));	

/*----------------- Verificando Perfil ---------------------*/ 
$perfis = arrayPerfil();
$boSuperUsuario = in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis );
$boPerfilPerm = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)));

/*
 * 
Function, salvar/editar
 */
function salva ($dicid = null, $pdcid = null, $excluir = null){
	global $db, $pdeid;
	
	
	if(!$excluir){
		if ($pdcid){
			$sql = sprintf("UPDATE
				 	 pdeescola.periododisciplinacritica
				    SET
				     pdcdatainicio = '%s',
				     pdecdatafim = '%s',
				     pdcanoreferencia = %d,
				     pdcdataatualizacao = %s
				    WHERE
				     pdcid = %d",
				$_POST['anoreferencia'].'/'.$_POST['mes_inicio'].'/01',
				$_POST['anoreferencia'].'/'.$_POST['mes_fim'].'/01',
				$_POST['anoreferencia'],
				'now()',
				$pdcid
			);
			$db->executar($sql);
		}else{
			$sql = sprintf("INSERT INTO pdeescola.periododisciplinacritica
					    (
					     pdeid,
					     pdcdatainicio,
					     pdecdatafim,
					     pdcanoreferencia,
					     pdcdataatualizacao
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 %d,
						 %s
					    )",
					$pdeid,
					$_POST['anoreferencia'].'/'.$_POST['mes_inicio'].'/01',
					$_POST['anoreferencia'].'/'.$_POST['mes_fim'].'/01',
					$_POST['anoreferencia'],
					'now()'
			);
			$db->PegaUm($sql); 
			$sql = "SELECT max(pdcid) as total
					FROM
					 pdeescola.periododisciplinacritica
					WHERE
					 pdeid = ".$pdeid;
			$pdcid = $db->PegaUm($sql);
		}
	}

	if ($dicid){
		
		if ($excluir){
			$sql = sprintf("DELETE from
						 	 pdeescola.disciplinacritica
						    WHERE
						     dicid = %d",
						$dicid
					);
					
		}else{
			$sql = sprintf("UPDATE
				 	 pdeescola.disciplinacritica
				    SET
				     tdiid = %d,
				     sceid = %d,
				     titid = %d,
				     dicturma = '%s',
				     dictaxareprovacao = %d
				    WHERE
				     dicid = %d",
				$_POST['tdiid'],
				$_POST['sceid'],
				$_POST['titid'],
				$_POST['dicturma'],
				$_POST['dictaxareprovacao'],
				$dicid
			);
		}
		
	}else{
		$sql = sprintf("INSERT INTO pdeescola.disciplinacritica
					    (
					     pdcid,
					     tdiid,
					     sceid,
					     titid,
					     dicturma,
					     dictaxareprovacao
					    )VALUES(
					     %d,
						 %d,
						 %d,
						 %d,
						 '%s',
						 %d
					    )",
					$pdcid,
					$_POST['tdiid'],
					$_POST['sceid'],
					$_POST['titid'],
					$_POST['dicturma'],
					$_POST['dictaxareprovacao']
				);
	}
	$db->executar($sql);
	
	if($excluir){
		# Se não tiver registro deletamos da tabela preenchimento 
	  	$sql2 = "SELECT * FROM pdeescola.periododisciplinacritica pd
					inner join pdeescola.disciplinacritica d on pd.pdcid = d.pdcid
				WHERE pd.pdeid = $pdeid";
	  	$arDisciplinaCritica = $db->carregar( $sql2 );
	  	if(!is_array($arDisciplinaCritica)){
	  		$pdeanoreferencia = $db->pegaUm("SELECT pdeanoreferencia FROM pdeescola.pdepreenchimento WHERE pdeid = '$pdeid' AND ppritem = 'p13'");
	  		if($pdeanoreferencia){
	  			$sql3 = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = 'p13' AND pdeanoreferencia = '".$pdeanoreferencia."'";
	  			$db->executar($sql3);
	  		}
	  	}
	}
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p13' AND
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

	if(!$excluir){
		$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p13'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento,  ppritem, pdeanoreferencia) VALUES ( '$pdeid','i1', 'p13', '{$_REQUEST['anoreferencia']}')";
			$db->executar( $sql_p ); 
		}
	}
		$db->commit(); 
 
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdcid = $_POST['pdcid'] ? $_POST['pdcid'] : $_GET['pdcid'];
$dicid = $_POST['dicid'] ? $_POST['dicid'] : $_GET['dicid'];
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
$cForm->direcPag('salva( dicid, pdcid)', array("dicid" => $dicid,
											   "pdcid" => $pdcid));

if($_REQUEST['Excluir']){
	salva($dicid, $pdcid, $_REQUEST['Excluir']);
}

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST){
	salva($dicid, $pdcid);
}else{
	if($_REQUEST['Excluir']){
		salva($dicid, $pdcid, $_REQUEST['Excluir']);
	}
}
*/

/*
 * Carrega dados
 */
$sql = "SELECT 
			pdcid, pdeid, pdcdatainicio, pdecdatafim, pdcanoreferencia, 
			pdcdataatualizacao, date_part('month', pdcdatainicio) as mes_inicio,  
			date_part('month', pdecdatafim) as mes_fim
		FROM
		 pdeescola.periododisciplinacritica
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);
extract($dados);

if($dicid){
	$sql = "SELECT
			 *
			FROM
			 pdeescola.disciplinacritica
			WHERE
			 dicid = ".$dicid;
	$dados = (array) $db->pegaLinha($sql);
	extract($dados);
}

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
<?=subCabecalho('13 - Disciplinas críticas'); ?>
<form action="" method="post" name="formulario">
<input type='hidden' name='dicid' value='<?=$dicid ?>' />
<input type='hidden' name='pdcid' value='<?=$pdcid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Ano de Referencia: :</td>
		<td width="60%">
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
		 
		<input type="hidden" name="pdcanoreferencia" value="<?=$ano?>">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período:</td>
		<td>
			<select name="mes_inicio" >
				<option value="">Escolha...</option>
				<option value="01" <?if($mes_inicio == '1') echo 'selected';?>>JANEIRO</option>
				<option value="02" <?if($mes_inicio == '2') echo 'selected';?>>FEVEREIRO</option>
				<option value="03" <?if($mes_inicio == '3') echo 'selected';?>>MARÇO</option>
				<option value="04" <?if($mes_inicio == '4') echo 'selected';?>>ABRIL</option>
				<option value="05" <?if($mes_inicio == '5') echo 'selected';?>>MAIO</option>
				<option value="06" <?if($mes_inicio == '6') echo 'selected';?>>JUNHO</option>
				<option value="07" <?if($mes_inicio == '7') echo 'selected';?>>JULHO</option>
				<option value="08" <?if($mes_inicio == '8') echo 'selected';?>>AGOSTO</option>
				<option value="09" <?if($mes_inicio == '9') echo 'selected';?>>SETEMBRO</option>
				<option value="10" <?if($mes_inicio == '10') echo 'selected';?>>OUTUBRO</option>
				<option value="11" <?if($mes_inicio == '11') echo 'selected';?>>NOVEMBRO</option>
				<option value="12" <?if($mes_inicio == '12') echo 'selected';?>>DEZEMBRO</option>
			</select>
			&nbsp;&nbsp;à&nbsp;&nbsp;
			<select name="mes_fim" >
				<option value="">Escolha...</option>
				<option value="01" <?if($mes_fim == '1') echo 'selected';?>>JANEIRO</option>
				<option value="02" <?if($mes_fim == '2') echo 'selected';?>>FEVEREIRO</option>
				<option value="03" <?if($mes_fim == '3') echo 'selected';?>>MARÇO</option>
				<option value="04" <?if($mes_fim == '4') echo 'selected';?>>ABRIL</option>
				<option value="05" <?if($mes_fim == '5') echo 'selected';?>>MAIO</option>
				<option value="06" <?if($mes_fim == '6') echo 'selected';?>>JUNHO</option>
				<option value="07" <?if($mes_fim == '7') echo 'selected';?>>JULHO</option>
				<option value="08" <?if($mes_fim == '8') echo 'selected';?>>AGOSTO</option>
				<option value="09" <?if($mes_fim == '9') echo 'selected';?>>SETEMBRO</option>
				<option value="10" <?if($mes_fim == '10') echo 'selected';?>>OUTUBRO</option>
				<option value="11" <?if($mes_fim == '11') echo 'selected';?>>NOVEMBRO</option>
				<option value="12" <?if($mes_fim == '12') echo 'selected';?>>DEZEMBRO</option>
			</select>

		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubTituloDireita">&nbsp;</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Disciplina:</td>
		<td>
		<?
			echo $db->monta_combo('tdiid',
                                "SELECT  tdiid AS codigo, 
									tdidescricao AS descricao
								FROM pdeescola.tipodisciplina
								ORDER BY tdidescricao",
                                'S', 'Escolha...', '', '', '', '', 'N', 'tdiid', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Série/Ciclo:</td>
		<td>
		<?
			$sql_serie_ciclo = "SELECT 
									sce.sceid AS codigo,
									(CASE sce.scetipo 
									WHEN 'C' THEN sce.scedescricao
									WHEN 'S' THEN sce.scedescricao || ' (' || tme.tmedescricao || ')'
									END) AS descricao
								FROM 
									pdeescola.seriecicloescolar sce
								INNER JOIN
									pdeescola.tiponivelmodalidadeensino tme ON tme.tmeid = sce.tmeid
								ORDER BY
									sce.sceid";
						
			echo $db->monta_combo('sceid', $sql_serie_ciclo, 'S', 'Escolha...', '', '', '', '', 'N', 'sceid', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Turma:</td>
		<td><?= campo_texto( 'dicturma', 'N', 'S', '', 70, 10, '', ''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Turno:</td>
		<td>
		<?
			echo $db->monta_combo('titid',
                                'SELECT  titid AS codigo, 
										 titdescricao AS descricao
								FROM pdeescola.tipoturno
								ORDER BY titdescricao',
                                'S', 'Escolha...', '', '', '', '', 'N', 'titid', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Taxa de Reprovação:</td>
		<td><?= campo_texto( 'dictaxareprovacao', 'N', 'S', '', 4, 3, '', '', '', '', 0, 'onkeypress="return somenteNumeros(event);"'); ?> %</td>
	</tr>
	<?php echo $cForm->montarbuttons("validaForm()");?>
	<tr>
		<td><br></td>
	</tr>			
</table>
<?
if( $estadoPerm || $boSuperUsuario )
{ 
	if($boSuperUsuario || $boPerfilPerm )
	{
	$sql_estado = "'<center><img style=\"cursor:pointer\" onclick=Atualizar(' || dc.dicid || ') src=\"/imagens/alterar.gif\" style=\"border:none\" alt=\"Alterar Item\" />
	<img style=\"cursor:pointer\" onclick=Excluir(' || dc.dicid || ') src=\"/imagens/excluir.gif\" style=\"border:none\" alt=\"Excluir Item\" /></center>' 
	as acao,";
    }else{
    	$sql_estado = ' \'\' as acao,';
    }
}
else{
	$sql_estado = ' \'\' as acao,';
}

//valor >= 5 ? "Maior ou igual a 5" : "Menor que 5
if($pdcid){
	$sql = 'SELECT
			'.$sql_estado.'
			td.tdidescricao,
			(CASE sc.scetipo 
			WHEN \'C\' THEN sc.scedescricao
			WHEN \'S\' THEN sc.scedescricao || \' (\' || tme.tmedescricao || \')\'
			END) AS scedescricao,
			dc.dicturma, 
			tp.titdescricao, 
			\'<center>\'||dc.dictaxareprovacao||\'%</center>\'
			FROM 
				pdeescola.disciplinacritica dc
			INNER JOIN 
				pdeescola.tipodisciplina td ON td.tdiid = dc.tdiid
			INNER JOIN 
				pdeescola.seriecicloescolar sc ON sc.sceid = dc.sceid
			INNER JOIN
				pdeescola.tiponivelmodalidadeensino tme ON tme.tmeid = sc.tmeid
			INNER JOIN 
				pdeescola.tipoturno tp ON tp.titid = dc.titid
			WHERE
			 dc.pdcid = '.$pdcid.'
			order by dc.dicid
			';
	
	$cabecalho = array( "Ação", 
						"Disciplinas",
						"Série/Ciclo",
						"Turma",
						"Turno",
						"Taxa de Reprovação");
	
	$db->monta_lista_simples( $sql, $cabecalho, 90, 10, 'N', '', '' );
}
?>

<script type="text/javascript">
d = document;
function validaForm(){

	if (d.formulario.elements['mes_inicio'].value == '') {
		alert("Campo 'Período: Mês Inicio' é Obrigatório.");
		return;
	}
	if (d.formulario.elements['mes_fim'].value == '') {
		alert("Campo 'Período: Mês Fim' é Obrigatório.");
		return;
	}
	if (d.formulario.elements['tdiid'].value == '') {
		alert("Campo 'Disciplina' é Obrigatório.");
		return;
	}
	if (d.formulario.elements['sceid'].value == '') {
		alert("Campo 'Série/Ciclo' é Obrigatório.");
		return;
	}
	if (d.formulario.elements['dicturma'].value == '') {
		alert("Campo 'Turma' é Obrigatório.");
		return;
	}
	if (d.formulario.elements['titid'].value == '') {
		alert("Campo 'Turno' é Obrigatório.");
		return;
	}
	if (d.formulario.elements['dictaxareprovacao'].value == '') {
		alert("Campo 'Taxa de Reprovação' é Obrigatório.");
		return;
	}


	d.formulario.submit();
	return true;
}

function Atualizar(id)
{
	location.href="pdeescola.php?modulo=principal/instrumento1/p13&acao=A&dicid="+id;
}

function Excluir(id)
{
	if(confirm('Deseja realmente excluir este item?'))
	{
		location.href="pdeescola.php?modulo=principal/instrumento1/p13&acao=A&Excluir=1&dicid="+id;
	}
}
</script>	