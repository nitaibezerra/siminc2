<?php
ini_set("memory_limit", "2048M");

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
		$db->monta_combo('muncod', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'muncod');
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

	function gerarXls(){
		if($('#estuf').val() == ''){
			alert('Por favor selecione o estado.');
			$('#estuf').focus();
		}
		else{
			selectAllOptions(document.getElementById('curid'));
			document.getElementById('tipo').value = 'xls';
			formulario.submit();
		}
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

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="tipo" id="tipo" value="" />
	<table id="" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<!--		<tr>-->
<!--			<td class="SubTituloDireita" width="10%">Profissionais:</td>-->
<!--			<td>-->
<!--				<input type="radio" name="tpeid" value="12" <?php if($_POST['tpeid'] == '12') echo 'checked="true"'?>> Docentes&nbsp;&nbsp;-->
<!--				<input type="radio" name="tpeid" value="2,7" <?php if($_POST['tpeid'] == '2,7') echo 'checked="true"'?>> Diretoria&nbsp;&nbsp;-->
<!--				<input type="radio" name="tpeid" value="8,9,10,11,14" <?php if($_POST['tpeid'] == '8,9,10,11,14') echo 'checked="true"'?>> Equipe pedagógica-->
<!--			</td>-->
<!--		</tr>-->
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
				<div style="float:right" >
					<input type="button" name="btn_pesquisar" value="Gerar Xls" onclick="gerarXls()"/>
				</div>
			</td>
		</tr>
	</table>
</form>
<?php 
	if($estuf){
		
		$where1 = Array("pde.estuf = ''$estuf''","wd.esdid = 417 ");
		$where2 = Array("pce.estuf = '$estuf'","doc.esdid = 464","pce.privagasprevistas >=1","pce.pristatus = 'A'");
		
		if($muncond){
			array_push($where1, " tm.muncod = ''$muncond''");
			array_push($where2, " m.muncod = '$muncond'");
		}
		if($pdiesfera){
			array_push($where1, " pde.pdiesfera = ''$pdiesfera''");
			array_push($where2, " pce.pdiesfera = '$pdiesfera'");
		}
		if($ateid){
			array_push($where1, " cur.ateid = $ateid");
			array_push($where2, " pce.ateid = $ateid");
		}
		if($_POST['curid'][0]){
			array_push($where1, " cur.curid IN (".implode(',', $_POST['curid']).")");
			array_push($where2, " pce.curid IN (".implode(',', $_POST['curid']).")");
		}
		
		$sql = "SELECT
					PD.pdicodinep,
					escola.pdenome,
					escola.pdiesfera, 
					escola.estuf, 
					Escola.mundescricao,
					escola.pdilocalizacao, 
					Escola.priidebi, 
					Escola.priidebf, 
					Escola.priordem, 
					PD.no_docente,
					PD.num_cpf,
					PD.no_email,
					PD.pfdprioridade,
					escola.atedesc, 
					escola.curid, 
					escola.curso,
					escola.pcfano
				FROM dblink('host= user= password= port= dbname=',
							'SELECT 
								pde.pdicodinep,
								doc.no_docente,
								doc.num_cpf,
								doc.no_email,
								--tpedesc, TIPO DOCENTE
								PFD.pfdprioridade
							FROM 
								pdeinterativo.planoformacaodocente pfd
							INNER JOIN educacenso_2010.tab_docente 			doc ON doc.pk_cod_docente = pfd.pk_cod_docente
							INNER JOIN educacenso_2010.tab_dado_docencia 	 dc ON dc.fk_cod_docente = doc.pk_cod_docente AND id_tipo_docente in (1)
							INNER JOIN catalogocurso.curso 					cur ON cur.curid = pfd.curid AND cur.curstatus = ''A''
							INNER JOIN pdeinterativo.pdinterativo 			pde ON pde.pdeid = pfd.pdeid AND pde.pdistatus = ''A''
							INNER JOIN territorios.municipio 				 tm ON tm.muncod = pde.muncod
							INNER JOIN workflow.documento 					 wd ON wd.docid = pde.formacaodocid
							INNER JOIN workflow.estadodocumento 			 ed ON ed.esdid = wd.esdid
							INNER JOIN pdeinterativo.periodocursoformacao 	pcf ON pcf.pcfid = pfd.pcfid
							WHERE 
								".implode(" AND ", $where1)."
							GROUP BY
								pde.estuf,
							 	pde.pdicodinep,
								doc.no_docente,
								doc.num_cpf,
								doc.no_email,
								PFD.pfdprioridade') 
							AS PD (
								pdicodinep varchar,
								no_docente varchar,
								num_cpf varchar,
								no_email varchar,
								pfdprioridade integer)
				INNER JOIN 
					(SELECT pce.pdicodinep inep, 
						pce.pdenome, 
						pce.priidebi, 
						pce.priidebf, 
						pce.pdiesfera, 
						pce.estuf, 
						m.mundescricao,
						pce.pdilocalizacao, 
						pce.atedesc, 
						pce.curid, 
						pce.curdesc curso,
						pce.privagasprevistas, 
						pce.priordem,
						pce.pcfano
					FROM 
						snf.prioridadecursoescola pce
					INNER JOIN territorios.municipio 	  m using(muncod)
					INNER JOIN snf.prioridadedocumento 	pdd using(prdid)
					INNER JOIN workflow.documento 		doc using(docid)
					WHERE 
						".implode(" AND ", $where2)."
					  	) as escola ON escola.inep = pd.pdicodinep
				ORDER BY 
					escola.priidebi ASC, escola.priidebf ASC, escola.priordem ASC, PD.pfdprioridade ASC";
		
		$cabecalho = array('INEP', 'Escola', 'Esfera', 'UF', 'Município', 'Localização	', 'IDEBf', 'IDEBi', 'Ordem', 
						   'Docente', 'CPF', 'E-mail', 'Prioridade', 'Area Temática', 'ID Curso', 'Curso', 'Ano');
		
		if( $_REQUEST['tipo'] == 'xls' ){
			ob_clean();
			header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
			header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
			header ( "Pragma: no-cache" );
			header ( "Content-type: application/xls; name=SIMEC_RelatProfCurso".date("Ymdhis").".xls");
			header ( "Content-Disposition: attachment; filename=SIMEC_RelatProfCurso".date("Ymdhis").".xls");
			header ( "Content-Description: MID Gera excel" );
			$db->monta_lista_tabulado($sql,$arCabecalho,100000,5,'N','100%',$par2);
			die;
		}
		
		$db->monta_lista($sql, $cabecalho, 20, 20, 'N', '70%');
	}
?>
</body>
</html>
