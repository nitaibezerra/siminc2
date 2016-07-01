<?php
	function comboMunicipios($estuf = null) {
		global $db;
		$estuf = !$estuf ? $_REQUEST['estuf'] : $estuf;
		$sql = "
			SELECT muncod       as codigo,
			       mundescricao as descricao
			FROM territorios.municipio 
			WHERE estuf = '".$estuf."'
			ORDER BY mundescricao
		";
		$db->monta_combo('muncod', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'muncod');
	}
	
	function comboUsuarios($pflcod = null) {
		global $db;
		$pflcod = !$pflcod ? $_REQUEST['pflcod'] : $pflcod;
		if($pflcod == 493){
			$sql = "
				SELECT usu.usucpf as codigo,
				       usu.usunome as descricao
				FROM seguranca.usuario usu
					INNER JOIN seguranca.perfilusuario pus ON pus.usucpf = usu.usucpf
				WHERE pus.pflcod = ".$pflcod."
				ORDER BY usu.usunome
			";
			$db->monta_combo('usucpf', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'usucpf');
		}
		else
			echo 'Disponível apenas para o perfil "Equipe MEC"';
	}

	if($_REQUEST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_REQUEST['requisicaoAjax']();
		exit;
	}

	if ( isset( $_REQUEST['buscar'] ) ) {
		include $_REQUEST['relatorio'];
		exit;
	}
	
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
	
	monta_titulo( "Relatórios Personalizados - PDEInterativo", "Para visualizar o relatório, basta selecionar e clicar no botão VISUALIZAR" );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function exibirRelatorio(p) {
		if(document.getElementById('relatorio').value!='') {
			var formulario = document.formulario;
			if(p == '0') {
				document.getElementById('visualizar').value='html';
				// submete formulario
				formulario.target = 'relatoriopersonlizadospdeinterativo';
				var janela = window.open( '', 'relatoriopersonlizadospdeinterativo', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
				formulario.submit();
				janela.focus();
			} else {
				document.getElementById('visualizar').value='xls';
				formulario.submit();
			}
	
		} else {
			alert("Selecione um relatório");
			return false;
		}
	}

	function comboMunicipios(estuf){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=comboMunicipios&estuf=" + estuf,
			success: function(retorno){
				$('#td_mun').html(retorno);
			}
		});
	}

	function comboUsuarios(pflcod){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=comboUsuarios&pflcod=" + pflcod,
			success: function(retorno){
				$('#td_usu').html(retorno);
			}
		});
	}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<input type="hidden" name="visualizar" id="visualizar" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Estado:</td>
			<td>
				<?php 
					$estuf = $_REQUEST['estuf'];
					$sql = "
						SELECT estuf        as codigo,
						       estdescricao as descricao
						FROM territorios.estado
						ORDER BY estdescricao asc
					";
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione', 'comboMunicipios', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Município:</td>
			<td id="td_mun">
				<?php
					if($estuf)
						comboMunicipios($estuf);
					else
						echo 'Por favor selecione o estado';												
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação:</td>
			<td>
			<?
			$sql_combo = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO."'";
			$db->monta_combo('esdid', $sql_combo, 'S', 'Selecione', '', '', '', '200', 'S', 'esdid', '', $_REQUEST['esdid']);
			?>
			</td>
		</tr>
		<!-- <tr>
			<td class="SubTituloDireita">Perfil:</td>
			<td>
				<?php
					/*$sql_combo = "
						SELECT pflcod as codigo,
						       pfldsc as descricao
						FROM seguranca.perfil
						WHERE pflstatus = 'A'
						AND   sisid     = ".$_SESSION['sisid']."
						ORDER BY pfldsc
					";
					$db->monta_combo('pflcod', $sql_combo, 'S', 'Selecione', 'comboUsuarios', '', '', '200', 'S', 'pflcod', '', $_REQUEST['pflcod']);*/
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Responsável:</td>
			<td id="td_usu">
				<?php
					/*if($pflcod)
						comboUsuarios($pflcod);
					else
						echo 'Disponível apenas para o perfil "Equipe MEC"';*/												
				?>
			</td>
		</tr> -->
		<tr>
			<td class="SubTituloDireita" valign="top">Relatórios :</td>
			<td>
			<?
			$relatorios = array(0=>array('codigo'=>'relatgeral_acompanhamento.inc','descricao'=>'Relatório de Acompanhamento'),
								1=>array('codigo'=>'relatgeral_saldo.inc','descricao'=>'Relatório de Saldo'));
			$db->monta_combo('relatorio', $relatorios, 'S', 'Selecione', '', '', '', '400', 'S', 'relatorio');
			?>
			</td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;">
			<input type="button" name="filtrar" value="Visualizar HTML" onclick="exibirRelatorio(0);"/> 
			<input type="button" name="filtrar" value="Visualizar XLS" onclick="exibirRelatorio(1);"/>
		</td>
		</tr>
</table>