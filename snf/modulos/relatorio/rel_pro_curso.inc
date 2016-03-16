<?php
	function comboMunicipios($estuf = null) {
		global $db;
		$estuf = !$estuf ? $_POST['estuf'] : $estuf;
		$sql = "
			SELECT muncod as codigo,
			       mundescricao as descricao
			FROM territorios.municipio
			WHERE estuf = '".$estuf."'
			ORDER BY mundescricao
		";
		$db->monta_combo('muncod', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'muncod');
	}
	
	function comboCursos($ateid = null) {
		global $db;
		$ateid = !$ateid ? $_POST['ateid'] : $ateid;
		$sql = "
			SELECT curid as codigo,
			       curdesc as descricao
			FROM catalogocurso.curso
			WHERE curstatus = 'A'
			AND   ateid     = ".$ateid."
			ORDER BY curdesc
		";
		combo_popup("curid", $sql, "Curso", "400x400", 0, '', "", "S", false, false, 5, 400);
	}
	
	function listarProfessores($estuf = null, $tpeid = null, $curid = null, $pdicodinep = null){
		global $db;
		$estuf = !$estuf ? $_POST['estuf'] : $estuf;
		$tpeid = !$tpeid ? $_POST['tpeid'] : $tpeid;
		$curid = !$curid ? $_POST['curid'] : $curid;
		$pdicodinep = !$pdicodinep ? $_POST['pdicodinep'] : $pdicodinep;
		$sql = "
			SELECT pesnome,
			       '<a mailto=\"'||lower(pfdemail)||'\">'||lower(pfdemail)||'</a>' AS pfdemail,
			       tpedesc,
			       pfdprioridade 
			FROM dblink(
				'host= user= password= port= dbname=', 
				'SELECT pes.pesnome as nome,
				        pfd.pfdemail as email,
				        tpe.tpedesc as descricao,
				        pfd.pfdprioridade
				 FROM pdeinterativo.pessoa pes
				 	INNER JOIN pdeinterativo.pessoatipoperfil     ptp ON ptp.pesid     = pes.pesid
				 	INNER JOIN pdeinterativo.tipoperfil           tpe ON tpe.tpeid     = ptp.tpeid 
					INNER JOIN pdeinterativo.pdinterativo         pde ON pde.pdeid     = ptp.pdeid
					AND                                                  pde.pdistatus = ''A'' 
					INNER JOIN territorios.municipio              mun ON mun.muncod    = pde.muncod 
					INNER JOIN pdeinterativo.planoformacaodocente pfd ON pfd.pesid     = pes.pesid
					AND                                                  pfd.pfdstatus = ''A'' 
					INNER JOIN catalogocurso.curso                cur ON cur.curid     = pfd.curid
					AND                                                  cur.curstatus = ''A'' 
					INNER JOIN workflow.documento                 doc ON doc.docid     = pde.formacaodocid 
					INNER JOIN workflow.estadodocumento           esd ON esd.esdid     = doc.esdid 
					INNER JOIN pdeinterativo.periodocursoformacao pcf ON pcf.pcfid     = pfd.pcfid 
				WHERE pes.pesstatus = ''A'' 
				AND   mun.estuf     = ''".$estuf."'' 
				AND   esd.esdid     = 417 
		";
		if($tpeid)
			$sql .= " AND tpe.tpeid     in (".$tpeid.")";
		if($curid) 
			$sql .= " AND cur.curid = ".$curid;
		if($pdicodinep)
			$sql .= " AND pde.pdicodinep = ''".$pdicodinep."''";
		$sql .= " ' 
			) AS PD ( 
				pesnome varchar, 
				pfdemail varchar, 
				tpedesc varchar, 
				pfdprioridade integer 
			)
		";
		$cabecalho = array('Nome', 'E-mail', 'Perfil', 'Prioridade');
		$db->monta_lista($sql, $cabecalho, 20, 20, 'N', '70%');
	}

	if($_REQUEST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_REQUEST['requisicaoAjax']();
		exit;
	}
	
	$estuf = !$estuf ? $_POST['estuf'] : $estuf;
	$muncod = !$muncod ? $_POST['muncod'] : $muncod;
	$pdiesfera = !$pdiesfera ? $_POST['pdiesfera'] : $pdiesfera;
	$ateid = !$ateid ? $_POST['ateid'] : $ateid;
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Relatório Profissionais por Curso', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript">
	function comboMunicipios(estuf){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=comboMunicipios&estuf=" + estuf,
			success: function(retorno){
				$('#td_muncod').html(retorno);
			}
		});
	}

	function comboCursos(ateid){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=comboCursos&ateid=" + ateid,
			success: function(retorno){
				$('#td_curid').html(retorno);
			}
		});
	}

	function pesquisar(){
		if($('#estuf').val() == ''){
			alert('Por favor selecione o estado.');
			$('#estuf').focus();
		}
		else{
			selectAllOptions(document.getElementById('curid'));
			formulario.submit();
		}
	}

	function listarProfessores(id, estuf, tpeid, curid, pdicodinep){
		$.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=listarProfessores&estuf=" + estuf + "&tpeid=" + tpeid + "&curid=" + curid + "&pdicodinep=" + pdicodinep,
			success: function(retorno){
				$('#'+id).html(retorno);
			}
		});
	}

	function abrirListaProfessores(cnt, estuf, tpeid, curid, pdicodinep) {
		var linha = cnt.parentNode.parentNode.parentNode;
		var tabela = cnt.parentNode.parentNode.parentNode.parentNode;
		if(tabela.rows.length != linha.rowIndex) {
			if(tabela.rows[linha.rowIndex].cells[1].id == "") {
				cnt.src = '../imagens/menos.gif';
				var linhan = tabela.insertRow(linha.rowIndex);
				var cell0 = linhan.insertCell(0);
				cell0.innerHTML = '&nbsp';
				var cell1 = linhan.insertCell(1);
				cell1.colSpan = 11;
				cell1.id = 'id_'+pdicodinep+'_'+linhan.rowIndex;
				cell1.innerHTML = 'Carregando...';
				listarProfessores(cell1.id, estuf, tpeid, curid, pdicodinep);
			} else {
				cnt.src = '../imagens/mais.gif';
				tabela.deleteRow(linha.rowIndex);
			}
		} else {
				cnt.src = '../imagens/menos.gif';
				var linhan = tabela.insertRow(linha.rowIndex);
				var cell0 = linhan.insertCell(0);
				cell0.innerHTML = '&nbsp';
				var cell1 = linhan.insertCell(1);
				cell1.colSpan = 11;
				cell1.id = 'id_'+pdicodinep+'_'+linhan.rowIndex;
				cell1.innerHTML = 'Carregando...';
				listarProfessores(cell1.id, estuf, tpeid, curid, pdicodinep);
		}
	}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<table id="" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Profissionais:</td>
			<td>
				<input type="radio" name="tpeid" value="12" <?php if($_POST['tpeid'] == '12') echo 'checked="true"'?>> Docentes&nbsp;&nbsp;
				<input type="radio" name="tpeid" value="2,7" <?php if($_POST['tpeid'] == '2,7') echo 'checked="true"'?>> Diretoria&nbsp;&nbsp;
				<input type="radio" name="tpeid" value="8,9,10,11,14" <?php if($_POST['tpeid'] == '8,9,10,11,14') echo 'checked="true"'?>> Equipe pedagógica
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Estado:</td>
			<td>
				<?php 
					$sql = "
						SELECT estuf AS codigo,
						       estdescricao AS descricao
						FROM territorios.estado
						ORDER BY estuf
					";
					$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'comboMunicipios', '', '', '140', 'S', 'estuf', false, $estuf);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Município:</td>
			<td id="td_muncod" >
				<?php
					if($estuf)
						comboMunicipios($estuf);
					else
						echo 'Por favor selecione o estado';												
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Esfera:</td>
			<td>
				<input type="radio" name="pdiesfera" value="Estadual" <?php if($pdiesfera == 'Estadual') echo 'checked="true"'?>> Estadual&nbsp;&nbsp;
				<input type="radio" name="pdiesfera" value="Municipal" <?php if($pdiesfera == 'Municipal') echo 'checked="true"'?>> Municipal
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Área temática:</td>
			<td>
				<?php 
					$sql = "
						SELECT ateid as codigo,
						       atedesc as descricao
						FROM catalogocurso.areatematica
						WHERE atestatus = 'A'
						ORDER BY atedesc
					";
					$db->monta_combo('ateid', $sql, 'S', 'Selecione', 'comboCursos', '', '', '300', 'N', 'ateid', false, $ateid);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Curso:</td>
			<td id="td_curid" >
				<?php
					if($ateid)
						comboCursos($ateid);
					else
						echo 'Por favor selecione a área temática';												
				?>
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2">
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisar()"/>
			</td>
		</tr>
	</table>
</form>
<?php 
	if($estuf){
		$sql = "
			SELECT '<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px;cursor:pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" onclick=\"abrirListaProfessores(this, \'".$estuf."\', \'".$_POST['tpeid']."\', \'".$curid."\', '||pri.pdicodinep||');\"></center>' as opcoes,
			       pri.pdicodinep,
			       pri.pdenome,
			       pri.pdiesfera,
			       pri.estuf,
			       mun.mundescricao,
			       pri.pdilocalizacao,
			       pri.atedesc,
			       pri.curdesc,
			       pri.privagasprevistas,
			       pri.priordem
			FROM snf.prioridadecursoescola pri 
				INNER JOIN territorios.municipio   mun ON mun.muncod = pri.muncod 
				INNER JOIN snf.prioridadedocumento prd ON prd.prdid  = pri.prdid
				INNER JOIN workflow.documento      doc ON doc.docid  = prd.docid 
			WHERE pri.pristatus         = 'A'
			AND   pri.privagasprevistas > 0
			AND   pri.estuf             = '".$estuf."'
			AND   pri.pcfano            in (2012, 2013) 
			AND   doc.esdid             = 464 
			AND   pri.curid             in (
				SELECT curid 
				FROM dblink(
					'host= user= password= port= dbname=', 
					'SELECT distinct cur.curid 
					 FROM pdeinterativo.pessoa pes
						 INNER JOIN pdeinterativo.pessoatipoperfil     ptp ON ptp.pesid     = pes.pesid
						 INNER JOIN pdeinterativo.tipoperfil           tpe ON tpe.tpeid     = ptp.tpeid
						 INNER JOIN pdeinterativo.pdinterativo         pde ON pde.pdeid     = ptp.pdeid
						 AND                                                  pde.pdistatus = ''A''
						 INNER JOIN territorios.municipio              mun ON mun.muncod    = pde.muncod
						 INNER JOIN pdeinterativo.planoformacaodocente pfd ON pfd.pesid     = pes.pesid
						 AND                                                  pfd.pfdstatus = ''A''
						 INNER JOIN catalogocurso.curso                cur ON cur.curid     = pfd.curid
						 AND                                                  cur.curstatus = ''A''
						 INNER JOIN workflow.documento                 doc ON doc.docid     = pde.formacaodocid
						 INNER JOIN workflow.estadodocumento           esd ON esd.esdid     = doc.esdid 
					WHERE pes.pesstatus = ''A'' 
					AND   mun.estuf     = ''".$estuf."''
					AND   esd.esdid     = 417'
				) AS PD (curid integer)
			)
		";
		if($muncond)
			$sql .= " AND pri.muncod     = ".$muncond;
		if($pdiesfera)
			$sql .= " AND pri.pdiesfera  = '".$pdiesfera."'";
		if($ateid)
			$sql .= " AND pri.ateid      = ".$ateid;
		if($_POST['curid'][0])
			$sql .= " AND pri.curid      in (".implode(',', $_POST['curid']).")"; 
		$sql .= "
			ORDER BY pri.estuf,
			         pri.muncod,
			         pri.curid,
			         pri.priordem 
		";
		$cabecalho = array('', 'INEP', 'Escola', 'Esfera', 'UF', 'Município', 'Localização	', 'Área Temática', 'Curso', 'Vagas Previstas', 'Priorização');
		$db->monta_lista($sql, $cabecalho, 20, 20, 'N', '70%');
	}
?>
</body>
</html>
