<?php 
$_SESSION['catalogo']['curid'] = $_REQUEST['curid'];;
?>
<html>
	<head>
		<title>Catálogo de Curso</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
	</head>
	<body>
		<?php 
		      
		if($_REQUEST['req']!=''){
			$_REQUEST['req']($_REQUEST);
		}
		
		$dadosCurso = recuperaDadosCurso($_REQUEST);
		?>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Dados Gerais
					</center>
				</td>
			</tr>
		</table>
		<input type="hidden" value="" id="req" name="req"/>
		<input type="hidden" value="" id="link" name="link"/>
		<input type="hidden" value="<?=$_SESSION['catalogo']['curid'] ?>" id="curid" name="curid"/>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Curso(Código)
				</td>
				<td>
					<?=$dadosCurso['curdesc'] ?>(<?=$dadosCurso['curid'] ?>)
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">
					Status
				</td>
				<td>
					<?=$dadosCurso['curstatus']=='A'?'Ativo':'Inativo'; ?>
				</td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr class="FundoTitulo">
				<td>
					<b>PARTE 1</b>
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Código / Área Temática
				</td>
				<td>
					<?=$dadosCurso['ateid']?> / <?=$db->pegaUm('SELECT atedesc FROM catalogocurso.areatematica WHERE ateid = '.$dadosCurso['ateid']); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Código / Nome do Curso
				</td>
				<td>
					<?=$dadosCurso['curid']?> / <?=$dadosCurso['curdesc'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Etapa de ensino a que se<br> destina
				</td>
				<td>
					<?php 
						if( is_array( $dadosCurso['cod_etapa_ensino'] ) ){
							foreach($dadosCurso['cod_etapa_ensino'] as $dado){
								echo $dado['descricao']."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Nivel do Curso
				</td>
				<td>
					<?=$db->pegaUm('SELECT ncudesc FROM catalogocurso.nivelcurso WHERE ncuid ='.$dadosCurso['ncuid']); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">
					Rede
				</td>
				<td>
					<?php 
						if( is_array($dadosCurso['redid']) ){
							$dados = $db->carregarColuna('SELECT reddesc FROM catalogocurso.rede WHERE redid in ('.implode(',',$dadosCurso['redid']).')'); 
	
							foreach($dados as $dado){
								echo $dado."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Objetivo
				</td>
				<td>
					<?=$dadosCurso['curobjetivo'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Ementa
				</td>
				<td>
					<?=$dadosCurso['curementa'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Fundamentos Teóricos Metodológicos
				</td>
				<td>
					<?=$dadosCurso['curfunteome'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Título Concedido /<br> Certificação
				</td>
				<td>
					<?=$dadosCurso['curcertificado'] ?>
				</td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr class="FundoTitulo">
				<td>
					<b>PARTE 2</b>
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Metodologia
				</td>
				<td>
					<?=$dadosCurso['curmetodologia'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Modalidade de Ensino
				</td>
				<td>
					<?php 
						$dadosCurso['modid'] = is_array($dadosCurso['modid']) ? $dadosCurso['modid'] : Array();
						$modalidades = $db->carregar('SELECT modid as codigo, moddesc as descricao FROM catalogocurso.modalidadecurso WHERE modid in ('.implode(',',$dadosCurso['modid']).')'); 
						$modids = Array();
						foreach($modalidades as $k => $modalidade){
							echo "<br> - ".$modalidade['descricao'];
							$modids[$k] = $modalidade['codigo']; 
						}
						echo "<br>	";
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Curso com Oferta Interestadual
				</td>
				<td>
					<?=$dadosCurso['curofertanacional']=='t'?'Sim':'Não' ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Carga Horária
				</td>
				<td>
					<br>
					<b>Carga horária do curso:</b><br>&nbsp;&nbsp;
					Mínimo:&nbsp;
					<?=$dadosCurso['curchmim'] ?> Horas.
					Máximo:&nbsp;
					<?=$dadosCurso['curchmax'] ?> Horas.<br><br>
					<?php if(in_array(1,$modids)){?>
						<b>Carga horária presencial exigida(%):</b><br>&nbsp;&nbsp;
						Mínimo:&nbsp;
						<?=$dadosCurso['curpercpremim'] ?> %.
						Máximo:&nbsp;
						<?=$dadosCurso['curpercpremax'] ?> %.<br>
					<?php }?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Número de estudantes por<br> turma
				</td>
				<td>
					<b>Presencial:</b><br>&nbsp;&nbsp;
					Ideal
					<?=$dadosCurso['curnumestudanteidealpre'] ?>.
					Mínimo
					<?=$dadosCurso['curnumestudanteminpre'] ?>.
					Máximo
					<?=$dadosCurso['curnumestudantemaxpre'] ?>.
					<br><br>
					<b>A Distância:</b><br>&nbsp;&nbsp;
					Ideal
					<?=$dadosCurso['curnumestudanteidealdist'] ?>.
					Mínimo
					<?=$dadosCurso['curnumestudantemindist'] ?>.
					Máximo
					<?=$dadosCurso['curnumestudantemaxdist'] ?>.
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Periodicidade de Monitoramento
				</td>
				<td>
					A cada 
					<?=$dadosCurso['curqtdmonitora'] ?>
					<?=$db->pegaUm('SELECT utedesc FROM catalogocurso.unidadetempo WHERE uteid ='.$dadosCurso['uteid']); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Infraestrutura Recomendada
				</td>
				<td>
					<?=$dadosCurso['curinfra'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Sala de Recursos Multifuncionais
				</td>
				<td>
					<?=$dadosCurso['cursalamulti']=='t' ? 'Sim' : 'Não' ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Localização da Escola
				</td>
				<td>
					<?=$db->pegaUm('SELECT lesdesc FROM catalogocurso.localizacaoescola WHERE lesid ='.$dadosCurso['lesid']); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Localização Diferenciada da Escola
				</td>
				<td>
					<?=$db->pegaUm('SELECT ldedesc FROM catalogocurso.localizacaodiferenciadaescola WHERE ldeid ='.$dadosCurso['ldeid']); ?>
				</td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr class="FundoTitulo">
				<td>
					<b>PARTE 3</b>
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Custo/Aluno estimado
				</td>
				<td>
					R$ &nbsp;
					<?=str_replace(Array('.',',','$'),Array('$','.',','),number_format($dadosCurso['curcustoaluno'],2)); ?>
				</td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
			<tr class="FundoTitulo">
				<td>
					<b>PARTE 4</b>
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Período de Oferta do Curso
				</td>
				<td>
					Início
					<?=$dadosCurso['curinicio'] ?>
					&nbsp;Fim
					<?=$dadosCurso['curfim'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Coordenação responsável no<br> MEC ou na CAPES
				</td>
				<td>
					<?=$db->pegaUm('SELECT coorddesc FROM catalogocurso.coordenacao WHERE coordid = '.$dadosCurso['coordid']); ?>
				</td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Organização do Curso
					</center>
				</td>
			</tr>
		</table>
		<?php 
			$sql = "SELECT 
						tiodesc, 
						orcdesc, 
						moddesc, 
						coalesce(orcchmim,0) as mim, 
					    coalesce(orcchmax,0) as max,
						coalesce(orcpercpremim,0)||' %' as permim, 
						coalesce(orcpercpremax,0)||' %' as permax, 
						--CASE WHEN LENGTH(orcementa) > 50
							--THEN SUBSTRING(orcementa FROM 0 FOR 50)||' ...'
							--ELSE 
							orcementa
						--END
					FROM 
						catalogocurso.organizacaocurso orc
					LEFT JOIN catalogocurso.tipoorganizacao tor ON tor.tioid = orc.tioid
					LEFT JOIN catalogocurso.modalidadecurso mod ON mod.modid = orc.modid
					WHERE
						orcstatus = 'A'
						AND orc.curid = ".$_REQUEST['curid']; 
			$cursos = $db->carregar($sql);
			$cabecalho = array("Tipo", "Nome", "Modalidade", "Hora Aula<br> (Mim.)", 
							   "Hora Aula<br> (Máx.)", "Carga Horária Presencial<br>Exigida % (Mim.)", "Carga Horária Presencial<br>Exigida % (max.)", "Descrição da Subdivisão");
			$db->monta_lista_array($cursos, $cabecalho, 50, 20, 'S', '100%', '',$arrayDeTiposParaOrdenacao);
		?>
		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Equipe
					</center>
				</td>
			</tr>
		</table>
		<?php 
			listaEquipes($_POST, false);
		?>
		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Público-Alvo
					</center>
				</td>
			</tr>
		</table>
		<?php 
		$dadosPublico = recuperaDadosPublicoAlvo($_REQUEST);
		?>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Função Exercida&nbsp;
				</td>
				<td>
					<?php 
						if( is_array($dadosPublico['fexid']) ){
							foreach($dadosPublico['fexid'] as $dados){
								echo $dados['descricao']."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Nivel de escolaridade permitido&nbsp;
				</td>
				<td>
					<?php 
						if( is_array($dadosPublico['cod_escolaridade']) ){
							foreach($dadosPublico['cod_escolaridade'] as $dado){
								echo $dado['descricao']."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr id="tr_formacao" style="display<?=in_array($dadosPublico['pacod_escolaridade'],Array(6,7))?"table-row":"none" ?>">
				<td class="SubTituloDireita">
					Area de Formação&nbsp;
				</td>
				<td>	
					<?php 
						if( is_array($dadosPublico['cod_area_ocde']) ){
							foreach($dadosPublico['cod_area_ocde'] as $dado){
								echo $dado['descricao']."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Disciplina(s) que leciona&nbsp;
				</td>
				<td>
					<?php 
						if( is_array($dadosPublico['cod_disciplina']) ){
							foreach($dadosPublico['cod_disciplina'] as $dado){
								echo $dado['descricao']."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Etapa de Ensino em que Leciona&nbsp;
				</td>
				<td>
					<?php 
						if( is_array($dadosPublico['cod_etapa_ensino']) ){
							foreach($dadosPublico['cod_etapa_ensino'] as $dado){
								echo $dado['descricao']."<br>";
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Modalidade em que leciona&nbsp;
				</td>
				<td>
					<?php 
						if( is_array($dadosPublico['cod_mod_ensino']) && count($dadosPublico['cod_mod_ensino']) ){
							$dados = $db->carregarColuna('SELECT no_mod_ensino FROM educacenso_2010.tab_mod_ensino WHERE pk_cod_mod_ensino in('.implode(',',$dadosPublico['cod_mod_ensino']).')');
							if( is_array($dados) ){
								foreach($dados as $dado){
									echo $dado."<br>";
								}
							}
						}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Outras Exigências&nbsp;
				</td>
				<td>
					<?=$dadosPublico['curpaoutrasexig'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Curso disponivel para demanda social?&nbsp;
				</td>
				<td>
					<?=$dadosPublico['curpademsocial']=='t'?'Sim':'Não' ?>
				</td>
			</tr>
			<?php if( $dadosPublico['curpademsocial']=='t' ){?>
			<tr class="tr_demsoc">
				<td class="SubTituloDireita">
					Percentual máximo de participantes na demanda social&nbsp;
				</td>
				<td>
					<?=$dadosPublico['curpademsocialpercmax'] ?>
					%
					<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
				</td>
			</tr>
			<tr class="tr_demsoc">
				<td class="SubTituloDireita">
					Público-alvo da demanda social&nbsp;
				</td>
				<td>
					<?php 
						if( is_array($dadosPublico['padid']) && count($dadosPublico['padid']) ){
							$dados = $dadosPublico['padid'];
							if( is_array($dados) ){
								foreach($dados as $dado){
									echo $dado['descricao']."<br>";
								}
							}
						}
					?>
				</td>
			</tr>
			<?php }?>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
			<tr class="SubTituloEsquerda">
				<td style="FONT-SIZE: 12pt;">
					<center>
						Contato no MEC ou Capes
					</center>
				</td>
			</tr>
		</table>
		<?php 
			$dadosContato = recuperaContato($_REQUEST);
		?>
		<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" width="15%">
					Telefone Principal&nbsp;
				</td>
				<td>
					<?=$dadosContato['curconttel'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Telefone Adicional&nbsp;
				</td>
				<td>
					<?=$dadosContato['curconttel2'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Nome&nbsp;
				</td>
				<td>
					<?=$dadosContato['curcontdesc'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					E-mail&nbsp;
				</td>
				<td>
					<?=$dadosContato['curcontemail'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Site&nbsp;
				</td>
				<td>
					<?=$dadosContato['curcontsite'] ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Outras Informações&nbsp;
				</td>
				<td>
					<?=$dadosContato['curcontinfo'] ?>
				</td>
			</tr>
			<tr>
				<td colspan="2" align="center">
					<input type="button" value="Imprimir" onclick="window.print();"/>
				</td>
			</tr>
		</table>
	</body>
</html>