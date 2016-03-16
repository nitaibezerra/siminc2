<?php 

if(!$_SESSION['sisfor']['sifid'])
	die("<script>
			alert('Problemas na navegação. Você esta sendo redirecionado apra página principal. Tente novamente!');
			window.location='sisfor.php?modulo=inicio&acao=C';
		 </script>");

$dadosprojeto = $db->pegaLinha("SELECT sifcargahorariapresencial, 
									   sifcargahorariadistancia, 
									   sifdddtelmatricula, 
									   siftelmatricula, 
									   sifemailmatricula, 
									   sifmetodologia, 
									   sifnumvagasofertadas, 
									   sifqtdvagas, 
									   sifprofmagisterio, 
									   sifprodmaterialdidatico, 
									   sifvigenciadtini, 
									   sifvigenciadtfim, 
									   siforigemrecursos, 
									   siftipocertificacao, 
									   case when s.ieoid is not null then cur.coordid 
											when s.cnvid is not null then cur2.coordid 
											when s.ocuid is not null then oc.coordid end as coordid
								FROM sisfor.sisfor s 
								LEFT JOIN catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
								LEFT JOIN catalogocurso2014.curso cur on cur.curid = ieo.curid 
								LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
								LEFT JOIN catalogocurso2014.curso cur2 on cur2.curid = cnv.curid 
								LEFT JOIN sisfor.outrocurso oc on oc.ocuid = s.ocuid
								WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");
if($dadosprojeto) extract($dadosprojeto);

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados Instituição/Projeto</td>
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
	
	<? 
	if($_SESSION['sisfor']['curid']) {
		$sql = "select curid, curdesc, curobjetivo, curementa from catalogocurso2014.curso where curid='".$_SESSION['sisfor']['curid']."'";
		$curso = $db->pegaLinha($sql);
	} else {

		$curso = $db->pegaLinha("SELECT ocu.ocuid as curid, 
										ocu.ocunome as curdesc, 
										ocuobjetivo as curobjetivo, 
										ocuementa as curementa, 
										ocucargahoraria as curchmax 
								 FROM sisfor.outrocurso ocu
							  	 INNER JOIN sisfor.sisfor sif ON sif.ocuid = ocu.ocuid
							  	 WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
	

	}
	
	
	
	?>
	
	
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados do Projeto</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
			<td><?=$curso['curid']." - ".$curso['curdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Objetivo do Curso</td>
		<td><?=$curso['curobjetivo'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Descrição do Curso</td>
		<td><?=$curso['curementa'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Metodologia</td>
		<td> <?=campo_textarea('sifmetodologia', 'N', 'N', 'Metodologia', '75', '4', '5000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Anexos</td>
		<td>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" width="100%">
		<tr>
			<td>
			<?php 
			
			$sql = "SELECT '<img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=window.location+\'&requisicao=downloadDocumentoDesignacao&arqid='||a.arqid||'\'\"> '||arqnome||'.'||arqextensao as nome, arqdescricao as descricao 
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
	<tr>
		<td class="SubTituloDireita" width="20%">Meta</td>
		<td><?=campo_texto('sifprofmagisterio', "N", "N", "Profissionais do magistério da Educação Básica", 7, 6, "######", "", '', '', 0, 'id="sifprofmagisterio"', '', $sifprofmagisterio ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Número de vagas ofertadas</td>
		<td><?=campo_texto('sifnumvagasofertadas', "N", "N", "Número de vagas ofertadas", 7, 6, "######", "", '', '', 0, 'id="sifnumvagasofertadas"', '', $sifnumvagasofertadas ); ?></td>
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
						<?=campo_data2('sifvigenciadtini','S', 'N', 'Início', 'S', '', '', '', '', '', 'sifvigenciadtini'); ?>
					</td>
					<td>
						<?=campo_data2('sifvigenciadtfim','S', 'N', 'Término', 'S', '', '', '', '', '', 'sifvigenciadtfim'); ?>
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
				WHERE curid = {$curso['curid']} AND fexstatus = 'A'";
		
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
					WHERE paf.curid={$curso['curid']}
					)
					UNION ALL
					(
					SELECT	no_pos_graduacao as no_escolaridade
					FROM catalogocurso2014.publicoalvo_assocnivelescolaridade pne
					INNER JOIN educacenso_2013.tab_pos_graduacao e ON (e.pk_pos_graduacao||'0')::integer = pne.nivelescolaridadeid
					INNER JOIN catalogocurso.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
					INNER JOIN catalogocurso2014.publicoalvo_assocfuncaoexercida paf ON paf.pafid = pne.pafid
					WHERE paf.curid={$curso['curid']}
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
		$db->monta_combo('siforigemrecursos', $_TIPO_AO, 'N', 'Selecione', '', '', '', '', 'S', 'siforigemrecursos'); 
		?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" width="20%">Carga horária presencial</td>
		<td><?=(($sifcargahorariapresencial)?$sifcargahorariapresencial:'0') ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Carga horária distância</td>
		<td><?=(($sifcargahorariadistancia)?$sifcargahorariadistancia:'0') ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Carga horária total</td>
		<td><?=($sifcargahorariapresencial+$sifcargahorariadistancia) ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Tipo de Certificação</td>
		<td><? 	
		$sql = "SELECT
				ncuid as codigo,
				ncuid||' - '||ncudesc as descricao
			FROM
				catalogocurso2014.nivelcurso
			WHERE
				ncustatus = 'A'";
		
		$db->monta_combo('siftipocertificacao', $sql, 'N', 'Selecione...', '', '', 'Tipo de Certificação', '', 'S', 'siftipocertificacao');
		 
		 ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Contatos para informações sobre matrícula</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">E-mail</td>
		<td><?=campo_texto('sifemailmatricula', "N", "N", "E-mail", 30, 100, "", "", '', '', 0, 'id="sifemailmatricula"', '', $sifemailmatricula ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Telefone</td>
		<td><?=campo_texto('sifdddtelmatricula', "N", "N", "DDD", 3, 3, "##", "", '', '', 0, 'id="sifdddtelmatricula"', '', $sifdddtelmatricula ); ?> <?=campo_texto('siftelmatricula', "N", "N", "Telefone", 20, 20, "#########", "", '', '', 0, 'id="siftelmatricula"', '', $siftelmatricula ); ?></td>
	</tr>
	
</table>
<?php 

$estruturacurso = $db->pegaLinha("SELECT * FROM sisfor.sisfor WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
if($estruturacurso) extract($estruturacurso);

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Estrutura do Curso</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Abrangência</td>
		<td>
		<p><b>Haverá polos? <input type="radio" name="haverapolos" disabled value="true" onclick="haveraPolos(this);" <?=(($sifpossuipolo=='t')?"checked":"") ?>> Sim <input type="radio" name="haverapolos" disabled value="false" onclick="haveraPolos(this);" <?=(($sifpossuipolo=='f')?"checked":"") ?>> Não</b></p>
		
		<div id="haverapolos_true" <?=(($sifpossuipolo=='t')?'':'style="display:none;"') ?>>
		<?
		
		listarPolosCurso(array('sifid' => $_SESSION['sisfor']['sifid'], "consulta" => true));
		
		?>
		</div>
		
		<div id="haverapolos_false" <?=(($estruturacurso['sifpossuipolo']=='f')?'':'style="display:none;"') ?>>
		
		<div id="div_abrangencia">
		<? definirAbrangencia(array("sifid"=>$_SESSION['sisfor']['sifid'],"consulta" => true)); ?>
		</div>
		
		</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Plano de Atividades</td>
		<td>
		<div id="dv_planoatividades">
		<? carregarPlanoAtividades(array("sifid" => $_SESSION['sisfor']['sifid'], "consulta" => true)); ?>
		</div>
		<br/>
		<p>Comentários sobre o cronograma (plano de atividades):</p>
		<?=$sifobsplanoatividades ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Articulação Institucional</td>
		<td>
			<table>
				<tr>
					<td>O Fórum Estadual Permanente conhece este projeto?</td>
					<td><input type="radio" name="sifforumestadualpermanente" value="TRUE" <?=(($sifforumestadualpermanente=="t")?"checked":"") ?> disabled> Sim <input type="radio" name="sifforumestadualpermanente" value="FALSE" <?=(($sifforumestadualpermanente=="f")?"checked":"") ?> disabled> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$sifforumestadualpermanentejustificativa ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a SEDUC?</td>
					<td><input type="radio" name="sifseduc" value="TRUE" <?=(($sifseduc=="t")?"checked":"") ?> disabled> Sim <input type="radio" name="sifseduc" value="FALSE" <?=(($sifseduc=="f")?"checked":"") ?> disabled> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$sifseducjustificativa ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNDIME?</td>
					<td><input type="radio" name="sifundime" value="TRUE" <?=(($sifundime=="t")?"checked":"") ?> disabled> Sim <input type="radio" name="sifundime" value="FALSE" <?=(($sifundime=="f")?"checked":"") ?> disabled> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$sifundimejustificativa ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNCME?</td>
					<td><input type="radio" name="sifuncme" value="TRUE" <?=(($sifuncme=="t")?"checked":"") ?> disabled> Sim <input type="radio" name="sifuncme" value="FALSE" <?=(($sifuncme=="f")?"checked":"") ?>  disabled> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$sifuncmejustificativa ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com movimentos sociais e outras organizações da sociedade civil?</td>
					<td><input type="radio" name="sifmsoc" value="TRUE" <?=(($sifmsoc=="t")?"checked":"") ?> disabled> Sim <input type="radio" name="sifmsoc" value="FALSE" <?=(($sifmsoc=="f")?"checked":"") ?> disabled> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=$sifoutrasarticulacoes ?></td>
				</tr>
				
				<tr>
					<td>Outras articulações</td>
					<td><?=$sifoutrasarticulacoes ?></td>
				</tr>
			</table>
		</td>
	</tr>

</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Equipe IES</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Quantitativo por perfil</td>
		<td>
		<? 
		
		carregarQuantitativoPorPerfil(array('sifid' => $_SESSION['sisfor']['sifid'], "consulta" => true));
		
		
		?>
		</td>
	</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		<div id="div_listacustos"><?
		carregarListaCustos(array("consulta" => true,"sifid"=>$_SESSION['sisfor']['sifid']));
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Custo Aluno (custeio)</td>
		<td>
		<? 
		calcularCustoAlunoCusteio(array("sifid"=>$_SESSION['sisfor']['sifid']));
		?>
		</td>
	</tr>
</table>

</td>
<td width="3%" valign="top">
<? wf_desenhaBarraNavegacao( $_SESSION['sisfor']['docidprojeto'], array('sifid' =>  $_SESSION['sisfor']['sifid']) ); ?>
</td>

</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
		<p>Compromete-se a/o <b><?=$db->pegaUm("select unidsc from public.unidade u inner join sisfor.sisfor s on s.unicod = u.unicod where s.sifid='".$_SESSION['sisfor']['sifid']."'") ?></b> a cumprir o objeto pactuado, buscando alcançar as metas pactuadas neste projeto, nos prazos e valores aqui definidos, salvo a ocorrência de eventos supervenientes. Quaisquer ocorrências que impliquem na alteração parcial deste projeto serão comunicadas ao Ministério da Educação com antecedência mínima de 30 (trinta) dias.</p>
		<p>A função gerencial fiscalizadora será exercida pelo MEC a partir dos dados encaminhados pela Instituição e/ou inseridos no Sistema de Gestão e Monitoramento da Formação Continuada.</p>
		<p>A Instituição está ciente de que, caso tenha algum projeto que apresente pendências no encaminhamento de informações que permitam o encerramento do mesmo, terá a análise de processos, novos repasses de créditos e pagamento de bolsas suspensos até que a situação seja regularizada.</p>		
		</td>
	</tr>
	<tr>
		<td>
		<p align="center">
		<br><br>___________________________________________________________________________________<br>
		<? echo $db->pegaUm("SELECT replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.')||' / '||iusnome FROM sisfor.sisfor s INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf WHERE sifid='".$_SESSION['sisfor']['sifid']."'"); ?> (Coordenador(a) do curso)
		</p>		
		</td>
	</tr>
</table>
