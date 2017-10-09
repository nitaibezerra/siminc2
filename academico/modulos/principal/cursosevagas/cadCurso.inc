<?php

	$curid = $_REQUEST['curid'];
	$entid = $_SESSION['academico']['entid'];
	$edita = false;
	if( $entidunidade = $_REQUEST['entidunidade']){
		$curid = $entidunidade;
		$edita = true;
	}
	$dados;
	
	if( empty($entid) ){
		echo "<script>history.back(-1);</script>";
	}

	if( $_REQUEST['cdtid'] ){
		$edita = true;
		$cdtid = $_REQUEST['cdtid'];
		
		$sql = "SELECT * FROM academico.cursodetalhe where cdtid = ".$cdtid;
		$dados = $db->pegaLinha( $sql );
		
		$sql = "SELECT * FROM public.curso WHERE curid = ".$dados['curid'];
		$dados2 = $db->pegaLinha( $sql );
	}
	
	if( $_POST['submete'] == 'true' ){

		$curdsc = $_POST['curdsc'];
		
		if( $_POST['cdtid'] ){
			$sql = "UPDATE public.curso SET
					curdsc='{$curdsc}', entid={$_POST['entidcampus']}, turid={$_POST['turidexecutado']}
					WHERE curid = ".$_POST['curidAtual'];
			$db->executar($sql);
			
			$duracao = $_POST['cdtduracao'] ? str_replace( ',', '.', $_POST['cdtduracao'] ) : 'null';
			$emec = $_POST['cdtcodigoemec'] ? $_POST['cdtcodigoemec'] : 'null';
			$sql = "UPDATE academico.cursodetalhe
					   SET curid={$_POST['curidAtual']}, cdtcodigoemec={$emec}, entid={$_POST['entidcampus']},
					   arcid={$_POST['arcid']}, turidexecutado={$_POST['turidexecutado']}, cdtduracao={$duracao}, cdtinicioexec={$_POST['cdtinicioexec']},
					   cdtobs='{$_POST['cdtobs']}'
					 WHERE cdtid = ".$_POST['cdtid'];
			$db->executar($sql);
			if($db->commit()){			
				echo '<script>alert("Operação realizada com sucesso!"); window.parent.opener.location.href = window.opener.location; window.close();</script>';
			} else {
				$db->insucesso('Falha na operação');
			}
		} else {
			
			if( $_REQUEST['entidunidade'] ){
				$cdtpactuacao =  'P';
				
				$arCampos = array('cdtvgprevano2007', 'cdtvgprevano2008', 'cdtvgprevano2009', 'cdtvgprevano2010', 'cdtvgprevano2011', 'cdtvgprevano2012' );
				
				foreach( $arCampos as $campo ){
					if( $_REQUEST[$campo] ){
						$valor = $_REQUEST[$campo];
						$campos.= ", ".$campo;
						$valores.= ", '".$valor."'";
					}
				}
			} else {
				$campos  = '';
				$valores = '';
				$cdtpactuacao =  'E';
			}
			
			$sql = "INSERT INTO
								public.curso ( curdsc, tpcid, entid, turid, curstatus )
						VALUES
								( '{$curdsc}', 1, {$_POST['entidcampus']}, {$_POST['turidexecutado']}, 'A' ) RETURNING curid";
			$curidN = $db->pegaUm($sql);
			if( $curidN ){
				$duracao = $_POST['cdtduracao'] ? str_replace( ',', '.', $_POST['cdtduracao'] ) : 'null';
				$emec = $_POST['cdtcodigoemec'] ? $_POST['cdtcodigoemec'] : 'null';
				
				$sql = "INSERT INTO
								academico.cursodetalhe ( curid, cdtcodigoemec, entid, pgcid, arcid, turidexecutado, cdtduracao, cdtinicioexec, cdtobs, stcid, cdtpactuacao, cdtdtinclusao, cdtstatus, nvcid {$campos} )
						VALUES
								( {$curidN}, {$emec}, {$_POST['entidcampus']}, {$_POST['pgcid']}, {$_POST['arcid']}, {$_POST['turidexecutado']}, {$duracao}, {$_POST['cdtinicioexec']}, '{$_POST['cdtobs']}', 1, '{$cdtpactuacao}', NOW(), 'A', 1 {$valores} )";
				$db->executar($sql);
			}
			if($db->commit()){			
				if( $_REQUEST['entidunidade'] ){
					$db->sucesso('');
				} else {
					$db->sucesso('principal/cursosevagas/cadCurso', '&tpcid=3&curid='.$curid);
				}
			} else {
				$db->insucesso('Falha na operação');
			}
		}
		die;
	}
	
	if ( $_REQUEST['grava'] ){
		$curidpactuado 	= $_REQUEST['curidpactuado'];
		$curidexecutado	= $_REQUEST['curidexecutado'];
		$sql = "SELECT ceqid FROM academico.cursoequivalencia WHERE curidpactuado = {$curidpactuado} AND curidexecutado = {$curidexecutado}";
		$teste = $db->pegaUm($sql);
		if($teste==''){
			$sql = "INSERT INTO academico.cursoequivalencia (curidpactuado, curidexecutado) values ($curidpactuado, $curidexecutado)";
			$db->executar($sql);
			$db->commit();
		}
		die;
	}
	
	if ( $_REQUEST['deleta'] ){
		$curidpactuado 	= $_REQUEST['curidpactuado'];
		$curidexecutado	= $_REQUEST['curidexecutado'];
		$sql = "SELECT ceqid FROM academico.cursoequivalencia WHERE curidpactuado = {$curidpactuado} AND curidexecutado = {$curidexecutado}";
		$teste = $db->pegaUm($sql);
		if($teste<>''){
			$sql = "DELETE FROM academico.cursoequivalencia WHERE curidpactuado = {$curidpactuado} AND curidexecutado = {$curidexecutado}";
			$db->executar($sql);
			$db->commit();
		}
		die;
	}
	
?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script language="javascript" 	type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<script type="text/javascript" 	src="../includes/JQuery/jquery2.js"></script>
		<script type="text/javascript" 	src="../includes/funcoes.js"></script>
		<script language="JavaScript" 	src="./geral/js/academico.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript">
			d = document;
			
			function fechar(){
				window.close();
			}
			
			function teste(){
				alert('teste');
			}
			
			function gravaCurso(curidexecutado, curidpactuado)
			{
				campo = document.getElementById( 'check_' + curidexecutado );
				if (campo.checked){
					$.ajax({
					   type: "POST",
					   url: "academico.php?modulo=principal/cursosevagas/cadCurso&acao=A",
					   data: "&grava=true&curidexecutado="+curidexecutado+"&curidpactuado="+curidpactuado,
					   success: function(msg){
					   }
					 });
				} else {
					$.ajax({
					   type: "POST",
					   url: "academico.php?modulo=principal/cursosevagas/cadCurso&acao=A",
					   data: "&deleta=true&curidexecutado="+curidexecutado+"&curidpactuado="+curidpactuado,
					   success: function(msg){
					   }
					 });
			   }
			}
			
			function abreCurso(){
				$('#cadNovoCurso').css('display', 'block');
				$('#listaCursos').css('display', 'none');
			}
			
			function salvarNovoCurso(){
				var txt = '';
				
				$('input').each(function (){
					$(this).val(trim($(this).val()));
				});
				
				if ($("#entidcampus").val() == '' ){
					txt += (txt ? '\n' : '') + 'O campo "Campus" deve ser preenchido!';
				}
				
				if ($("input[name='curdsc']").val() == '' ){
					txt += (txt ? '\n' : '') + 'O campo "Nome" deve ser preenchido!';
				}
				
				if ($("select[name='pgcid']").val() == '' ){
					txt += (txt ? '\n' : '') + 'O campo "Programa" deve ser preenchido!';
				}
				
				if ($("select[name='arcid']").val() == '' ){
					txt += (txt ? '\n' : '') + 'O campo "Área" deve ser preenchido!';
				}
	
				if ($("select[name='turidexecutado']").val() == '' ){
					txt += (txt ? '\n' : '') + 'O campo "Turno" deve ser preenchido!';
				}
	
				if ($("input[name='cdtinicioexec']").val() == '' ){
					txt += (txt ? '\n' : '') + 'O campo "Início" deve ser preenchido!';
				}
				 
				
				if (txt != ''){
					alert(txt);
					return false;
				}
				
				$('#submete').val('true');				
			}
			
		</script>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
	<?php 
	
monta_titulo("Cadastro de Curso<br><font size='2'>$tipoCurso</font>", obrigatorio() . ' Indica campo obrigatório' );

$dadoTd = 'align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"';
?>
	<input type="hidden" name="evento" id="evento" value="manterEdital">
	<input type="hidden" name="curid" id="curid" value="<?=$curid?>">
	<input type="hidden" name="cdtid" id="cdtid" value="<?=$cdtid?>">
	<!-- 
	<input type="hidden" name="entid" id="entid" value="<?=$entid?>">
	 -->
	<input type="hidden" name="tpcid" id="tpcid" value="<?=$tpcid?>">
		<div id="cadNovoCurso" style="display: none;">
			<form method="POST"  name="formulario" onsubmit="javascript:return salvarNovoCurso();" enctype="multipart/form-data">
				<input type="hidden" name="curidAtual" id="curidAtual" value="<?=$dados['curid']?>">
				<input type="hidden" name="evento" id="evento" value="manterEdital">
				<input type="hidden" name="curid" id="curid" value="<?=$curid?>">
				<input type="hidden" name="cdtid" id="cdtid" value="<?=$cdtid?>">
				<input type="hidden" name="tpcid" id="tpcid" value="<?=$tpcid?>">
				<input type="hidden" name="submete" id="submete" value="">
				<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				        <td align='right' class="SubTituloDireita">Código e-MEC:</td>
				        <td>
							<?=campo_texto('cdtcodigoemec','N',$habilPac,'',10,20,'','','','','','','',$dados['cdtcodigoemec']);?>
				        </td>
				      </tr>
				      <tr>
				        <td align='right' class="SubTituloDireita">Nome:</td>
				        <td>
							<?=campo_texto('curdsc','S','S','',60,100,'','','','','','','',$dados2['curdsc']);?>
				        </td>
				      </tr>
				 <tr>
				<td class="SubTituloDireita">Campus:</td>
				<?
				$sql = "SELECT
								e.entid AS codigo,
								e.entnome AS descricao
							FROM
								entidade.entidade e2
							INNER JOIN 
								entidade.entidade e ON e2.entid = e.entid
							INNER JOIN 
								entidade.funcaoentidade ef ON e.entid = ef.entid
							INNER JOIN 
								entidade.funentassoc ea ON ef.fueid = ea.fueid 
							-- INNER JOIN 
							--	academico.entidadeportaria ep ON ea.entid = ep.entid 
							-- INNER JOIN 
							--	academico.lancamentosportaria lp ON e.entid = lp.entidcampus 
							WHERE 
								e.entstatus = 'A' 
								AND ea.entid = " . $entid . "
								AND ef.funid = 18 
							GROUP BY 
								e.entid, 
								e.entnome";
				?>
				<td><?=$db->monta_combo("entidcampus",$sql, 'S','-- Selecione --', '', '', '', 250, 'S','entidcampus', '', $dados['entid'], 'Campus'); ?></td>
				 </tr>   
				 <tr>
				<td class="SubTituloDireita">Programa:</td>
				<?
				$sql = "SELECT 
				 pgcid as codigo,
				 pgcdsc as descricao
				FROM 
				 academico.programacurso
				order by pgcdsc";
				if( $_REQUEST['cdtid'] ){
				$prog = $db->pegaLinha( $sql );
				?>
				<td><?=$prog['descricao']?></td>
				<?php } else { ?>
				<td><?=$db->monta_combo("pgcid",$sql, 'S','-- Selecione --', '', '', '', 250, 'S','pgcid','',$dados['pgcid']); ?></td>
				<?php } ?>
				 </tr>  
				 <tr>
				<td class="SubTituloDireita">Área:</td>
				<?
				$sql = "SELECT 
				 arcid as codigo,
				 arcdsc as descricao
				FROM 
				 academico.areacurso
				order by arcdsc";
				?>
				<td><?=$db->monta_combo("arcid",$sql, 'S','-- Selecione --', '', '', '', 250, 'S','arcid','',$dados['arcid']); ?></td>
				 </tr>  
				      <tr>
				        <td align='right' class="SubTituloDireita">Duração:</td>
				        <td>
							<?=campo_texto('cdtduracao','N','S','',5,5,'##,##','','','','','','',$dados['cdtduracao'],'this.value=mascaraglobal(\'##,##\', this.value);');?> Anos
				        </td>
				      </tr>  
				 <tr>
				<td class="SubTituloDireita">Turno:</td>
				<?
				$sql = "SELECT 
				 turid as codigo,
				 turdsc as descricao
				FROM 
				 academico.turno
				order by turdsc";
				?>
				<td><?=$db->monta_combo(turidexecutado, $sql, 'S','-- Selecione --', '', '', '', 250, 'S','turidexecutado','',$dados['turidexecutado']); ?></td>
				 </tr>
				      <tr>
				        <td align='right' class="SubTituloDireita">Ano de Início:</td>
				        <td>
							<?=campo_texto('cdtinicioexec','S','S','',6,4,'####','','','','','','',$dados['cdtinicioexec']);?>
				        </td>
				      </tr>  
				 <tr>
				<td class="SubTituloDireita">Observação:</td>
				<td><?=campo_textarea('cdtobs','N', 'S', '', 80, 05, 200,'','','','','',$dados['cdtobs']); ?></td>
				 </tr>
				 <?php if( $entidunidade = $_REQUEST['entidunidade']){ ?>
					 <tr>
						<td class="SubTituloDireita">Previsto:</td>
						<td><table class="tabela" bgcolor="#f5f5f5" width="50%" align="center" border="0" cellspacing="0" cellpadding="2">
							<thead><tr colspan='100%'>
					  			<td <?php echo $dadoTd; ?>><strong>2007</strong></td>
					  			<td <?php echo $dadoTd; ?>><strong>2008</strong></td>
					  			<td <?php echo $dadoTd; ?>><strong>2009</strong></td>
					  			<td <?php echo $dadoTd; ?>><strong>2010</strong></td>
					  			<td <?php echo $dadoTd; ?>><strong>2011</strong></td>
					  			<td <?php echo $dadoTd; ?>><strong>2012</strong></td>
							</thead></tr>
							<tr bgcolor="#f5f5f5">
				  				<td align="center"> <?php echo campo_texto( 'cdtvgprevano2007' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , '' ); ?> </td>
				  				<td align="center"> <?php echo campo_texto( 'cdtvgprevano2008' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , '' ); ?> </td>
				  				<td align="center"> <?php echo campo_texto( 'cdtvgprevano2009' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , '' ); ?> </td>
				  				<td align="center"> <?php echo campo_texto( 'cdtvgprevano2010' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , '' ); ?> </td>
				  				<td align="center"> <?php echo campo_texto( 'cdtvgprevano2011' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , '' ); ?> </td>
				  				<td align="center"> <?php echo campo_texto( 'cdtvgprevano2012' , 'N', 'S', '', 6, 4, '####', '', '', '', '', '' ,'' , '' ); ?> </td>
				  			</tr>
					  	</table></td>
					  </tr>
				 <?php } ?>
				<tr colspan='3' bgcolor="#d0d0d0">
				  <td>
					<input name="salvar" type="submit" value="Salvar">
				</td>
				<td>
					<input type="button" name="btFechar" value="Fechar" onclick="window.parent.opener.location.href = window.opener.location; window.close();" style="cursor: pointer;">
				</td>
				 </tr>  
				</table>
			</form>
		</div>
		<div id="listaCursos">
	<?php 
	
		$sql = "SELECT DISTINCT
					case when
						(select count(x.ceqid) from academico.cursoequivalencia x where x.curidpactuado = {$curid} AND x.curidexecutado = cd.curid) > 0
					then
						'<center>
							<input type=checkbox checked value='||c.curid||' name=check_'||c.curid||' id=check_'||c.curid||' onclick=\"gravaCurso('||  c.curid ||', $curid );\">
						</center>'
					else
						'<center>
							<input type=checkbox value='||c.curid||' name=check_'||c.curid||' id=check_'||c.curid||' onclick=\"gravaCurso('||  c.curid ||', $curid );\">
						</center>'
					end as acao,
					curdsc, 
					entnome,
					te.turdsc
				FROM 
					academico.cursodetalhe cd
				LEFT JOIN
					academico.cursoequivalencia eq ON eq.curidexecutado = cd.curid
				INNER JOIN 
					public.curso c ON c.curid = cd.curid
				INNER JOIN
					entidade.entidade e ON e.entid = cd.entid
				LEFT JOIN 
					academico.turno te ON te.turid = cd.turidexecutado
				WHERE
					cdtpactuacao = 'E'
					AND cdtstatus = 'A'
					AND e.entid IN (SELECT
											e.entid
										FROM
											entidade.entidade e2
										INNER JOIN 
											entidade.entidade e ON e2.entid = e.entid
										INNER JOIN 
											entidade.funcaoentidade ef ON e.entid = ef.entid
										INNER JOIN 
											entidade.funentassoc ea ON ef.fueid = ea.fueid 
										--INNER JOIN 
										--	academico.entidadeportaria ep ON ea.entid = ep.entid 
										--INNER JOIN 
										--	academico.lancamentosportaria lp ON e.entid = lp.entidcampus
										WHERE 
											e.entstatus = 'A' 
											AND ea.entid = {$entid}
											AND ef.funid = 18
										GROUP BY 
											e.entid, 
											e.entnome)
					AND cd.curid NOT IN ( select x.curidexecutado from academico.cursoequivalencia x where x.curidpactuado = {$curid} )
				ORDER BY
					entnome, curdsc";
		

			$alinha = array("center","left","left","left");
			$tamanho = array("10%","40%","40%", "10%");
			$db->monta_lista( $sql, array( "Ação", "Nome do Curso", "Campus", "Turno" ), 15, 10, 'N', 'center', '','', $tamanho,$alinha);
	
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#d0d0d0">
			<td>
				<input type="button" name="btFechar" value="Fechar" onclick="window.parent.opener.location.href = window.opener.location; window.close();" style="cursor: pointer;">
			</td>
			<td>
				<input type="button" name="btNovo" value="Novo Curso" onclick="abreCurso();" style="cursor: pointer;">
			</td>
		</tr>
	</table>
	</div>
</body>
<?php if ( $edita == true ){ ?>
<script>
	abreCurso();
</script>
<?php } ?>
</html>