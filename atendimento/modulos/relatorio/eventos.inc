<?php 
	function comboMunicipios($estuf = null) {
		global $db;
		$estuf = !$estuf ? $_REQUEST['estuf'] : $estuf;
		$muncod = $_REQUEST['muncod'];
		$sql = "
			SELECT muncod       as codigo,
			       mundescricao as descricao
			FROM territorios.municipio 
			WHERE estuf = '".$estuf."'";
		combo_popup( 'muncod['. $muncod .']', $sql, 'Selecione o(s) município(s)', '360x460' );
	}

	if($_REQUEST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_REQUEST['requisicaoAjax']();
		exit;
	}
	
	include APPRAIZ . 'includes/cabecalho.inc';
	echo '<br/>';
	monta_titulo( 'Relatório de Eventos', '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">
	function relatorio(evtid){
		var form = document.formulario;
		$('#evtid').val(evtid);
		$('#visualizar').val('relatorio');
		form.submit();
	}
	
	function pesquisar(){
		var form = document.formulario;
		$('#visualizar').val('pesquisa');
		prepara_formulario();
		form.submit();
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
</script>
<form name="formulario" id="pesquisar" method="POST" action="">
	<input type="hidden" name="evtid" id="evtid" value="">
	<input type="hidden" name="visualizar" id="visualizar" value="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data Inicial:</td>
			<td>
				<?php
					$data_inicio = '';
					if($_POST['evtdataini'] != '')
						$data_inicio = ajusta_data($_POST['evtdataini']);
					echo campo_data('evtdataini', 'N', 'S', '', 'S', '', '',$data_inicio);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data Final:</td>
			<td>
				<?php
					$data_fim = '';
					if($_POST['evtdatafim'] != '')
						$data_fim = ajusta_data($_POST['evtdatafim']);
					echo campo_data('evtdatafim', 'N', 'S', '', 'S', '', '',$data_fim);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Assunto:</td>
			<td>
				<?php
					echo campo_texto('evtassunto', "N", "S", "Assunto", 100, 250, "", "", '', '', 0, 'id="evtassunto"', '', $_REQUEST['evtassunto'] );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Presença de Parlamentares:</td>
			<td>
				<input type="radio" name="parlamentares" value="S" <?php if($_REQUEST['parlamentares'] == 'S') echo 'checked' ?>>
				Sim
				<input type="radio" name="parlamentares" value="N" <?php if($_REQUEST['parlamentares'] == 'N') echo 'checked' ?>>
				Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UFs Relacionadas:</td>
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
			<td class="SubTituloDireita">Municípios Relacionados:</td>
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
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="button" value="Pesquisar" onClick="pesquisar()" />
			</td>
		</tr>
	</table>
</form>
<?php 
	if($_REQUEST['visualizar'] == 'relatorio'){
		$sql = "
			SELECT agd.agddescricao,
			       evt.evtassunto,
			       to_char(evt.evtdataini, 'DD/MM/YYYY') as evtdataini,
			       to_char(evt.evtdatafim, 'DD/MM/YYYY') as evtdatafim,
			       evt.evtpauta
			FROM atendimento.evento evt
				INNER JOIN atendimento.agenda agd ON agd.agdid = evt.agdid
			WHERE evt.evtid = ".$_REQUEST['evtid']."
		";
		$evento = $db->pegaLinha($sql);
?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" width="50%"><b>Agenda:</b></td>
				<td><?php echo $evento['agddescricao']?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="50%"><b>Assunto:</b></td>
				<td><?php echo $evento['evtassunto']?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="50%"><b>Período:</b></td>
				<td><?php echo $evento['evtdataini']." a ".$evento['evtdatafim'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2"><b>Estados / Municípios Relacionados:</b></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2">
<?php 
					$sql = "
						SELECT est.estdescricao,
						       mun.mundescricao
						FROM atendimento.eventovinculos evv
							LEFT JOIN territorios.estado    est ON est.estuf  = evv.estuf
							LEFT JOIN territorios.municipio mun ON mun.muncod = evv.muncod
						WHERE evv.evtid     = ".$_REQUEST['evtid']."
						AND   evv.evvstatus = 'A'
						ORDER BY est.estdescricao,
						         mun.mundescricao
					";
					$cabecalho = array('Estado', 'Município');
					$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');
?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2"><b>Participantes Internos:</b></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2">
<?php 
					$sql = "
						SELECT pes.pesnome,
						       pes.pesemail,
						       pev.pevcargo,
						       pes.pestelefone
						FROM atendimento.evento evt
							INNER JOIN atendimento.pessoaevento pev ON pev.evtid = evt.evtid
							INNER JOIN atendimento.pessoa       pes ON pes.pesid = pev.pesid
						WHERE evt.evtid     = ".$_REQUEST['evtid']."
						AND   pev.pevstatus = 'A'
						AND   pev.tppid     = 1
					";
					$cabecalho = array('Nome', 'E-mail', 'Cargo', 'Telefone');
					$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');
?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2"><b>Participantes Externos:</b></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2">
<?php 
					$sql = "
						SELECT pes.pesnome,
						       pes.pesemail,
						       pev.pevcargo,
						       pes.pestelefone,
						       pev.estuf,
						       mun.mundescricao,
						       pev.pevorgao
						FROM atendimento.evento evt
							INNER JOIN atendimento.pessoaevento pev ON pev.evtid = evt.evtid
							INNER JOIN atendimento.pessoa       pes ON pes.pesid = pev.pesid
							LEFT  JOIN territorios.municipio    mun ON mun.muncod = pev.muncod
						WHERE evt.evtid     = ".$_REQUEST['evtid']."
						AND   pev.pevstatus = 'A'
						AND   pev.tppid     = 2
					";
					$cabecalho = array('Nome', 'E-mail', 'Cargo', 'Telefone', 'UF', 'Município', 'Órgão');
					$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');
?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2"><b>Parlamentares:</b></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2">
<?php 
					$sql = "
						SELECT pes.pesnome,
						       pes.pesemail,
						       pev.pevcargo,
						       pes.pestelefone,
						       pev.estuf
						FROM atendimento.evento evt
							INNER JOIN atendimento.pessoaevento pev ON pev.evtid = evt.evtid
							INNER JOIN atendimento.pessoa       pes ON pes.pesid = pev.pesid
							LEFT  JOIN territorios.municipio    mun ON mun.muncod = pev.muncod
						WHERE evt.evtid     = ".$_REQUEST['evtid']."
						AND   pev.pevstatus = 'A'
						AND   pev.tppid     = 2
					";
					$cabecalho = array('Nome', 'E-mail', 'Cargo', 'Telefone', 'UF');
					$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');
?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="50%"><b>Pauta do Evento:</b></td>
				<td><?php echo $evento['evtpauta']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="50%"><b>Resumo:</b></td>
				<td><?php echo $evento['evtobservacoes']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2"><b>Encaminhamentos:</b></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="50%" colspan="2">
<?php
					$sql = "
						SELECT ecm.ecmdescricao,
						       pes.pesnome,
						       to_char(ecm.ecmprazo, 'DD/MM/YYYY') as ecmprazo,
						       CASE WHEN pec.pecstatus = 'C'
						           THEN 'Sim'
								   ELSE 'Não'
							   END AS pecstatus
						FROM atendimento.encaminhamento ecm
							INNER JOIN atendimento.pevencaminhamento pec ON pec.ecmid = ecm.ecmid
							INNER JOIN atendimento.pessoaevento      pev ON pev.pevid = pec.pevid
							INNER JOIN atendimento.pessoa      		 pes ON pes.pesid = pev.pesid
						WHERE pev.evtid     = ".$_REQUEST['evtid']."
						AND   ecm.ecmstatus = 'A'
						ORDER BY ecm.ecmid
					";
					$cabecalho = array('Encaminhamento', 'Responsável', 'Prazo', 'Concluído');
					$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');
?>
				</td>
			</tr>
		</table>	
<?php 
	}
	else{
		$sql = "
			SELECT '<center><img src=\"/imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"relatorio(' || evt.evtid || ');\"></center>' as acao,
			       agd.agddescricao,
			       evt.evtassunto,
			       to_char(evt.evtdataini, 'DD/MM/YYYY') as evtdataini,
			       to_char(evt.evtdatafim, 'DD/MM/YYYY') as evtdatafim
			FROM atendimento.evento evt
				INNER JOIN atendimento.agenda         agd ON agd.agdid = evt.agdid
			WHERE evt.evtstatus <> 'I'
		";
		if($_REQUEST['evtdataini'] != '')
			$sql .= "
				AND evt.evtdataini BETWEEN '".formata_data_sql($_REQUEST['evtdataini'])." 00:00:00' AND '".formata_data_sql($_REQUEST['evtdataini'])." 23:59:59'
			";
		if($_REQUEST['evtdatafim'] != '')
			$sql .= "
				AND evt.evtdatafim BETWEEN '".formata_data_sql($_REQUEST['evtdatafim'])." 00:00:00' AND '".formata_data_sql($_REQUEST['evtdatafim'])." 23:59:59'
			";
		if($_REQUEST['evtassunto'] != '')
			$sql .= "
				AND evt.evtassunto like '%".str_replace(' ', '%', $_REQUEST['evtassunto'])."%'
			";
		if($_REQUEST['parlamentares'] != ''){
			$operador = '>';
			if($_REQUEST['parlamentares'] == 'N')
				$operador = '=';
			$sql .= "
				AND (SELECT count(pevid)
				     FROM atendimento.pessoaevento
				     WHERE tppid     = 4
				     AND   pevstatus = 'A'
				     AND   evtid     = evt.evtid) ".$operador." 0
			";
		}
		if($_REQUEST['estuf'] != ''){
			$sql .= "
				AND (SELECT count(evvid)
				     FROM atendimento.eventovinculos
				     WHERE evtid     = evt.evtid
				     AND   evvstatus = 'A'
				     AND   estuf     = '".$_REQUEST['estuf']."') > 0
			";
		}
		$_REQUEST['muncod'] = is_array( $_REQUEST['muncod'] ) ? $_REQUEST['muncod'] : array();
		$list_muncod = '';
		foreach ($_REQUEST['muncod'] as $muncod => $mun)
			$list_muncod .= "'" . $mun[0] . "',";
		$list_muncod = substr($list_muncod, 0, -1);
		if($list_muncod != ''){
			$sql .= "
				AND (SELECT count(evvid)
				     FROM atendimento.eventovinculos
				     WHERE evtid     = evt.evtid
				     AND   evvstatus = 'A'
				     AND   muncod    in (".$list_muncod.")) > 0
			";
		}
		$sql .= "
			ORDER BY agd.agddescricao
		";
		$cabecalho = array('Ação', 'Agenda', 'Assunto', 'Data Inicial', 'Data Final');
		$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '95%');
	}
?>