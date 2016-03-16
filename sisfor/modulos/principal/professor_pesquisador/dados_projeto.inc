<?php 

if(!$_SESSION['sisfor']['sifid'])
	die("<script>
			alert('Problemas na navegação. Você esta sendo redirecionado apra página principal. Tente novamente!');
			window.location='sisfor.php?modulo=inicio&acao=C';
		 </script>");


$estado = wf_pegarEstadoAtual( $_SESSION['sisfor']['docidprojeto'] );

if($estado['esdid'] != ESD_PROJETO_EMCADASTRAMENTO || in_array(PFL_CONSULTAGERAL,$perfil)) {
	$consulta = true;
}

$dadosprojeto = $db->pegaLinha("SELECT sifcargahorariapresencial, sifcargahorariadistancia, sifdddtelmatricula, siftelmatricula, sifemailmatricula, sifmetodologia, sifnumvagasofertadas, sifqtdvagas, sifprofmagisterio, sifprodmaterialdidatico, sifvigenciadtini, sifvigenciadtfim, siforigemrecursos, siftipocertificacao FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
if($dadosprojeto) extract($dadosprojeto);

$sifcargahoraria = ($sifcargahorariapresencial+$sifcargahorariadistancia);

if($_SESSION['sisfor']['curid']) {
	$sql = "select cur.ncuid, cur.curid, cur.curdesc, cur.curobjetivo, cur.curementa, cur.curcertificado, cur.curchmim, cur.curchmax from catalogocurso2014.curso cur where cur.curid = ".$_SESSION['sisfor']['curid']." and cur.curstatus = 'A'";
	$arr = $db->pegaLinha($sql);
	if($arr) extract($arr);
	
	if(!($sifcargahorariapresencial+$sifcargahorariadistancia)) $sifcargahoraria = ($curchmim+$curchmax);
}

if(!$curid) {
	
	$arr = $db->pegaLinha("SELECT ocu.ocuid as curid, ocu.ocunome as curdesc, ocuobjetivo, ocuementa, ocucargahoraria FROM sisfor.outrocurso ocu
						  INNER JOIN sisfor.sisfor sif ON sif.ocuid = ocu.ocuid
						  WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
	
	if($arr) extract($arr);
	
	$curobjetivo = campo_textarea('ocuobjetivo', 'S', (($consulta)?'N':'S'), 'Objetivo', '75', '4', '5000');
	$curementa = campo_textarea('ocuementa', 'S', (($consulta)?'N':'S'), 'Ementa', '75', '4', '5000');
	
}


?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>

function atualizarDadosProjeto() {

	<? if($_SESSION['sisfor']['curid']) : ?>

	if(jQuery('#sifcargahoraria').val() != '<?=($curchmim+$curchmax) ?>') {
		alert('Carga horária não pode ser maior do que <?=($curchmim+$curchmax) ?>');
		return false;
	}

	<? endif; ?>

	if(jQuery('#sifprodmaterialdidatico').val() > jQuery('#sifcargahoraria').val()) {
		alert('Carga horária para produção de material didático esta maior que a carga horária do curso');
		return false;
	}

	if(parseInt(jQuery('#sifnumvagasofertadas').val()) < parseInt(jQuery('#sifprofmagisterio').val())) {
		alert('campo "Número de vagas ofertadas" só pode ser igual ou superior ao valor do campo "Meta"');
		return false;
	}
	
	if(jQuery('#sifvigenciadtini').val()=='') {
		alert('Preencha Vigência do projeto - Início');
		return false;
	}

	if(jQuery('#sifvigenciadtfim').val()=='') {
		alert('Preencha Vigência do projeto - Término');
		return false;
	}

	if(!validaData(document.getElementById('sifvigenciadtini'))) {
		alert('Início - Vigência do projeto inválida');
		return false;
	}

	if(!validaData(document.getElementById('sifvigenciadtfim'))) {
		alert('Fim - Vigência do projeto inválida');
		return false;
	}
	
	var data1    = jQuery('#sifvigenciadtini').val();
	data1 		 = data1.split('/');
	var data2    = jQuery('#sifvigenciadtfim').val();
	data2 		 = data2.split('/');
	
	if(data2[2]+data2[1]+data2[0] < data1[2]+data1[1]+data1[0]) {
		alert('Data de Inicio de Vigência do projeto é maior do que Data Fim');
		return false;
	}
	
	if(jQuery('#siforigemrecursos').val()=='') {
		alert('Selecione Origem dos recursos');
	}

	if(jQuery('#siftipocertificacao').val()=='') {
		alert('Selecione Tipo de certificação');
	}

	jQuery('#formulario').submit();
	
}

function somarCargaHoraria() {
	var chpresencial = 0;
	if(jQuery("#sifcargahorariapresencial").val()!='') {
		chpresencial = parseInt(jQuery("#sifcargahorariapresencial").val());
	}

	var chdistancia = 0;
	if(jQuery("#sifcargahorariadistancia").val()!='') {
		chdistancia = parseInt(jQuery("#sifcargahorariadistancia").val());
	}
	
	jQuery("#sifcargahoraria").val(chpresencial+chdistancia);
}

function anexarProjetoCurso() {
	if(jQuery('#anexoprojetocurso').val()=='') {
		alert('Selecione um arquivo');
		return false;	
	}
	
	jQuery('#requisicao').val('anexarProjetoCurso');
    divCarregando();
	document.getElementById('formulario').submit();
}

function removerAnexoProjetoCurso(apcid) {
	var conf = confirm('Deseja realmente excluir este anexo?');
	
	if(conf) {
		divCarregando();
		window.location=window.location+'&requisicao=removerAnexoProjetoCurso&apcid='+apcid;
	}
}

</script>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" id="requisicao" value="atualizarDadosProjeto">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados Instituição/Projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td>Orientações</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados da Instituição</td>
	</tr>
	<?php 
	
	$sql = "SELECT e.entid, e.entnumcpfcnpj, e.entnome, e.entsig, en.endcep, en.estuf, m.mundescricao, en.endlog, en.endbai, en.endcom, en.endnum, e.entnumcomercial, e.entnumdddcomercial, e.entemail FROM entidade.entidade e 
			INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid ANd f.funid='".FUN_UNIVERSIDADE."'
			LEFT JOIN entidade.endereco en ON en.entid = e.entid
			LEFT JOIN territorios.municipio m ON m.muncod = en.muncod
			WHERE entunicod='".$_SESSION['sisfor']['unicod']."'";
	
	$instituicao = $db->pegaLinha($sql);
	
	?>
	<tr>
		<td class="SubTituloDireita" width="20%">CNPJ</td>
		<td><?=campo_texto('entnumcpfcnpj', "N", "N", "CNPJ", 20, 20, "##.###.###/####-##", "", '', '', 0, 'id="unicnpj"', '', mascaraglobal($instituicao['entnumcpfcnpj'],"##.###.###/####-##"),'if(this.value.length==18){carregaCNPJ();}'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da Instituição</td>
		<td><?=campo_texto('entnome', "N", "N", "Nome da Instituição", 67, 150, "", "", '', '', 0, 'id="entnome"', '', $instituicao['entnome']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Sigla</td>
		<td><?=campo_texto('entsig', "N", "N", "Sigla", 15, 10, "", "", '', '', 0, 'id="entsig"', '', $instituicao['entsig']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Endereço</td>
		<td>
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td align="right" width="10%">CEP</td>
					<td><?=campo_texto('endcep', "N", "N", "CEP", 9, 10, "#####-###", "", '', '', 0, 'id="endcep"', '', mascaraglobal($instituicao['endcep'],"#####-###"),'if(this.value.length==9){carregarEnderecoPorCEP_dirigente(comp.dados.nu_cep);}'); ?></td>
				</tr>
				<tr>
					<td align="right">UF</td>
					<td><?=$instituicao['estuf'] ?></td>
				</tr>
				<tr>
					<td align="right">Município</td>
					<td><?=$instituicao['mundescricao'] ?></td>
				</tr>
				<tr>
					<td align="right">Logradouro</td>
					<td><?=campo_texto('endlog', "N", "N", "Logradouro", 60, 150, "", "", '', '', 0, 'id="endlog"', '', $instituicao['endlog'] ); ?></td>
				</tr>
				<tr>
					<td align="right">Bairro</td>
					<td><?=campo_texto('endbai', "N", "N", "Bairro", 60, 150, "", "", '', '', 0, 'id="endbai"', '', $instituicao['endbai'] ); ?></td>
				</tr>
				<tr>
					<td align="right">Complemento</td>
					<td><?=campo_texto('endcom', "N", "N", "Complemento", 60, 150, "", "", '', '', 0, 'id="endcom"', '', $instituicao['endcom'] ); ?></td>
				</tr>
				<tr>
					<td align="right">Número</td>
					<td><?=campo_texto('endnum', "N", "N", "Número", 6, 5, "#####", "", '', '', 0, 'id="endnum"', '', $instituicao['endnum'] ); ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Contato</td>
		<td>
			<table cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td align="right" width="10%">Telefone</td>
					<td><?=campo_texto('entnumdddcomercial', "N", "N", "DDD", 3, 3, "##", "", '', '', 0, 'id="entnumdddcomercial"', '', $instituicao['entnumdddcomercial'] ); ?> <?=campo_texto('entnumcomercial', "N", "N", "Telefone", 20, 20, "####-####", "", '', '', 0, 'id="entnumcomercial"', '', $instituicao['entnumcomercial'] ); ?></td>
				</tr>
				<tr>
					<td align="right" width="10%">E-mail</td>
					<td><?=campo_texto('entemail', "N", "N", "E-mail", 60, 60, "", "", '', '', 0, 'id="entemail"', '', $instituicao['entemail']); ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<?php 
	
	if($instituicao['entid']) {

		$sql = "select ent.entnumcpfcnpj, ent.entnome, ent.entnumdddcomercial, ent.entnumcomercial, ent.entemail from entidade.funentassoc fea 
				inner join entidade.funcaoentidade fue on fue.fueid = fea.fueid 
				inner join entidade.funcao fun on fun.funid = fue.funid 
				inner join entidade.entidade ent on ent.entid = fue.entid
				where fea.entid=".$instituicao['entid']." and fue.funid=21";
		
		$reitor = $db->pegaLinha($sql);
		
	}
	
	?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Dirigente</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CPF</td>
		<td><?=campo_texto('entnumcpfcnpj', "N", "N", "CPF", 17, 14, "###.###.###-##", "", '', '', 0, 'id="entnumcpfcnpj"', '', mascaraglobal($reitor['entnumcpfcnpj'],"###.###.###-##"),'if(this.value.length==14){carregaCPF();}'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome</td>
		<td><?=campo_texto('entnome', "N", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="entnome"', '', $reitor['entnome']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Telefone</td>
		<td><?=campo_texto('entnumdddcomercial', "N", "N", "DDD", 3, 3, "##", "", '', '', 0, 'id="entnumdddcomercial"', '', $reitor['entnumdddcomercial'] ); ?> <?=campo_texto('entnumcomercial', "N", "N", "Telefone", 20, 20, "####-####", "", '', '', 0, 'id="entnumcomercial"', '', $reitor['entnumcomercial'] ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">E-mail</td>
		<td><?=campo_texto('entemail', "N", "N", "E-mail", 60, 60, "", "", '', '', 0, 'id="entemail"', "", $reitor['entemail']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
			<td><?=$curid." - ".$curdesc ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Objetivo do Curso</td>
		<td><?=$curobjetivo ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Descrição do Curso</td>
		<td><?=$curementa ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Comentários</td>
		<td> <?=campo_textarea('sifmetodologia', 'N', (($consulta)?'N':'S'), 'Metodologia', '75', '4', '5000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Anexos</td>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" width="100%">
		<? if(!$consulta) : ?>
		<tr>
			<td>
				<input type="file" name="arquivo" id="anexoprojetocurso"> <?=campo_texto('arqdescricao', "S", "S", "Descrição", 30, 50, "", "", '', '', 0, 'id="arqdescricao"', '', '' ); ?><br><input type="button" value="Anexar" onclick="anexarProjetoCurso();">
			</td>
			
		</tr>
		<? endif; ?>
		<tr>
			<td>
			<?php 
			
			$sql = "SELECT '<img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=window.location+\'&requisicao=downloadDocumentoDesignacao&arqid='||a.arqid||'\'\"> ".((!$consulta)?"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerAnexoProjetoCurso('||p.apcid||')\"> ":"")."'||arqnome||'.'||arqextensao as nome, arqdescricao as descricao 
					FROM sisfor.anexoprojetocurso p
					INNER JOIN public.arquivo a ON a.arqid = p.arqid 
					WHERE sifid='".$_SESSION['sisfor']['sifid']."'";
			
			$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
			
			
			?>
			</td>
			
		</tr>
		</table>
		</td>
	</tr>
	<? if(!$sifprofmagisterio) $sifprofmagisterio = $sifqtdvagas; ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Meta</td>
		<td><?=campo_texto('sifprofmagisterio', "S", (($consulta)?'N':'S'), "Profissionais do magistério da Educação Básica", 7, 6, "######", "", '', '', 0, 'id="sifprofmagisterio"', '', $sifprofmagisterio ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de vagas ofertadas</td>
		<td><?=campo_texto('sifnumvagasofertadas', "S", (($consulta)?'N':'S'), "Número de vagas ofertadas", 7, 6, "######", "", '', '', 0, 'id="sifnumvagasofertadas"', '', $sifnumvagasofertadas ); ?></td>
	</tr>
	

	<tr>
		<td class="SubTituloDireita" width="20%">Vigência do projeto</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
				<tr>
					<td class="SubTituloCentro">Início</td>	
					<td class="SubTituloCentro">Término</td>			
				</tr>
				<tr>
					<td>
						<?=campo_data2('sifvigenciadtini','S', (($consulta)?'N':'S'), 'Início', 'S', '', '', '', '', '', 'sifvigenciadtini'); ?>
					</td>
					<td>
						<?=campo_data2('sifvigenciadtfim','S', (($consulta)?'N':'S'), 'Término', 'S', '', '', '', '', '', 'sifvigenciadtfim'); ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<? if($_SESSION['sisfor']['curid']) : ?>	
	<tr>
		<td class="SubTituloDireita" width="20%">Público Alvo</td>
		<td>
		<?
		$sql = "SELECT '<img src=../imagens/seta_filho.gif> '||fe.fexdesc FROM catalogocurso2014.publicoalvo_assocfuncaoexercida paf
				LEFT JOIN catalogocurso2014.funcaoexercida fe ON fe.fexid = paf.fexid
				WHERE curid = {$curid} AND fexstatus = 'A'";
		
		$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
		?>
		</td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" width="20%">Requisitos para Participação</td>
		<td>
		<?
		$sql = "SELECT DISTINCT '<img src=../imagens/seta_filho.gif> '||foo.no_escolaridade FROM (
		
					(
					SELECT no_escolaridade as no_escolaridade FROM catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					INNER JOIN educacenso_2013.tab_escolaridade ne ON ne.pk_cod_escolaridade = pne.nivelescolaridadeid
					INNER JOIN catalogocurso2014.publicoalvo_assocfuncaoexercida paf ON paf.pafid = pne.pafid
					WHERE paf.curid={$curid}
					)
					UNION ALL
					(
					SELECT	no_pos_graduacao as no_escolaridade
					FROM catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					INNER JOIN educacenso_2013.tab_pos_graduacao e ON (e.pk_pos_graduacao||'0')::integer = pne.nivelescolaridadeid
					INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
					INNER JOIN catalogocurso2014.publicoalvo_assocfuncaoexercida paf ON paf.pafid = pne.pafid
					WHERE paf.curid={$curid}
					)
				) foo";
		
		
		$db->monta_lista_simples($sql,array(),50,5,'N','100%',$par2);
		
		?>
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Origem dos recursos</td>
		<td><? 
		$_TIPO_AO = array(0 => array("codigo"=>"A","descricao"=>"Ação Orçamentária"),
						  1 => array("codigo"=>"D","descricao"=>"Descentralização"),
						  2 => array("codigo"=>"C","descricao"=>"Convênio"),
						  3 => array("codigo"=>"P","descricao"=>"Plano de Ações Articuladas - PAR"));
		$db->monta_combo('siforigemrecursos', $_TIPO_AO, (($consulta)?'N':'S'), 'Selecione', '', '', '', '', 'S', 'siforigemrecursos'); 
		?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" width="20%">Produção de Material Didático p/ "X" Horas</td>
		<td><?=campo_texto('sifprodmaterialdidatico', "N", (($consulta)?'N':'S'), "Produção de  Material Didático p/ X Horas", 7, 6, "######", "", '', '', 0, 'id="sifprodmaterialdidatico"', '', $sifprodmaterialdidatico ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Carga horária presencial</td>
		<td><?=campo_texto('sifcargahorariapresencial', 'S', (($consulta)?'N':'S'), 'Carga Horária', 7, 6, "######", "", '', '', 0, 'id="sifcargahorariapresencial"', 'somarCargaHoraria();', ((!is_null($sifcargahorariapresencial))?$sifcargahorariapresencial:$curchmim) ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Carga horária distância</td>
		<td><?=campo_texto('sifcargahorariadistancia', 'S', (($consulta)?'N':'S'), 'Carga Horária', 7, 6, "######", "", '', '', 0, 'id="sifcargahorariadistancia"', 'somarCargaHoraria();', ((!is_null($sifcargahorariadistancia))?$sifcargahorariadistancia:$curchmax) ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Carga horária total</td>
		<td><?=campo_texto('sifcargahoraria', 'S', 'N', 'Carga Horária', 7, 6, "######", "", '', '', 0, 'id="sifcargahoraria"', '', $sifcargahoraria ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Tipo de Certificação</td>
		<td><? 
		
		if(!$siftipocertificacao) $siftipocertificacao = $ncuid;

		$sql = "SELECT
				ncuid as codigo,
				ncuid||' - '||ncudesc as descricao
			FROM
				catalogocurso2014.nivelcurso
			WHERE
				ncustatus = 'A'";
		
		$db->monta_combo('siftipocertificacao', $sql, (($consulta)?'N':'S'), 'Selecione...', '', '', 'Tipo de Certificação', '', 'S', 'siftipocertificacao');
		 ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Contatos para informações sobre matrícula</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">E-mail</td>
		<td><?=campo_texto('sifemailmatricula', "N", (($consulta)?'N':'S'), "E-mail", 30, 100, "", "", '', '', 0, 'id="sifemailmatricula"', '', $sifemailmatricula ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Telefone</td>
		<td><?=campo_texto('sifdddtelmatricula', "N", (($consulta)?'N':'S'), "DDD", 3, 3, "##", "", '', '', 0, 'id="sifdddtelmatricula"', '', $sifdddtelmatricula ); ?> <?=campo_texto('siftelmatricula', "N", (($consulta)?'N':'S'), "Telefone", 20, 20, "#########", "", '', '', 0, 'id="siftelmatricula"', '', $siftelmatricula ); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<? if(!$consulta) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="atualizarDadosProjeto('sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dados_projeto');"> 
		<? endif; ?>
		</td>
	</tr>
</table>
</form>