<?

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>';


monta_titulo('Planejamento 2015','Lista geral');

?>

<form id="formulario" method="post" name="formulario" action="">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="25%">Orientações:</td>
			<td>[ORIENTACOES]</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Secretaria:</td>
			<td>
			<?php 
			$sql = "SELECT coordid as codigo, coordsigla as descricao FROM catalogocurso2014.coordenacao WHERE coorano='".$_SESSION['exercicio']."' ORDER BY coordid";
			$db->monta_combo('coordid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'coordid', '', $_REQUEST['coordid']);
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Universidade:</td>
			<td>
			<?php
			
			$sql = "SELECT 	DISTINCT ent.entunicod as codigo, COALESCE(ent.entsig||' - ','')||ent.entnome as descricao
					FROM 		entidade.entidade ent
					INNER JOIN 	entidade.funcaoentidade fun ON fun.entid = ent.entid AND funid IN (12,11) 
					INNER JOIN sisfor.sisfories si ON si.unicod = ent.entunicod 
					WHERE ent.entstatus='A' AND ent.entunicod IS NOT NULL 
					ORDER BY 2";
			
			$db->monta_combo('unicod', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'unicod', '', $_REQUEST['unicod']); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome do curso:</td>
			<td>
			<?php 
			$nomecurso = $_REQUEST['nomecurso'];
			echo campo_texto('nomecurso', 'N', 'S', 'Nome do curso', '50', '', '', '','',''); 
			?>
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="submit" value="Pesquisar"/>
				<input type="button" value="Ver todos" id="btnTodos" onclick="window.location='sisfor.php?modulo=principal/mec/planejamento2015&acao=A';"/>
			</td>
		</tr>
	</table>
</form>

<form method="post" id="formulario2" name="formulario2">
<input type="hidden" name="requisicao" value="atualizarPlanejamento2015">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
	<tr>
		<td colspan="2">
		<div id="resumoPlanejamento2015">
		<? 
		resumoPlanejamento2015(array('coordid' 	 => $_REQUEST['coordid'],
									 'unicod' 	 => $_REQUEST['unicod'],
									 'nomecurso' => $_REQUEST['nomecurso'])); 
		?>
		</div>
		<?
		
		if($_REQUEST['coordid']) {
			$wh[] = "(cor.coordid='".$_REQUEST['coordid']."' or cor2.coordid='".$_REQUEST['coordid']."' or cor3.coordid='".$_REQUEST['coordid']."' or cor4.coordid='".$_REQUEST['coordid']."')";
			$wh2[] = "p.lcosecretaria in(select coordsigla from catalogocurso2014.coordenacao where coordid='".$_REQUEST['coordid']."')";
		}
		
		if($_REQUEST['unicod']) {
			$wh[] = "p.unicod='".$_REQUEST['unicod']."'";
			$wh2[] = "p.unicod='".$_REQUEST['unicod']."'";
		}
		
		if($_REQUEST['nomecurso']) {
			$wh[] = "(cur.curid||' '||cur.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR cur2.curid||' '||cur2.curdesc ilike '%".$_REQUEST['nomecurso']."%' OR oc.ocunome ilike '%".$_REQUEST['nomecurso']."%' OR oat.oatnome ilike '%".$_REQUEST['nomecurso']."%')";
			$wh2[] = "p.lconome ilike '%".$dados['nomecurso']."%'";
		}
		
		
		
		
		$sql = "select 
					'<span style=font-size:x-small;>'||foo.unicod||'</span>' as unicod,
					'<span style=font-size:x-small;>'||foo.universidade||'</span>' as universidade,
					'<span style=font-size:x-small;>'||foo.lconome||'</span>' as lconome,
					'<span style=font-size:x-small;>'||foo.secretaria||'</span>' as secretaria,
					'<span style=font-size:x-small;>'||foo.lcoano||'</span>' as lcoano,
					'<span style=font-size:x-small;float:right;>'||foo.sifprofmagisterio||'</span>' as sifprofmagisterio,
					'<span style=font-size:x-small;float:right;>'||trim(coalesce(to_char(foo.valortotal,'999g999g999d99'),''))||'</span>' as valortotal,
					'<span style=font-size:x-small;float:right;>'||case when (foo.valortotal-valorempenhado)=0 then '0,00' else trim(coalesce(to_char(foo.valortotal-valorempenhado,'999g999g999d99'),''))||'</span>' end as valornaoempenhado,
					foo.aa
				from (
					
				select 
					u.unicod, 
					uniabrev||' - '||unidsc as universidade,
					lconome as lconome, 
					case when s.ieoid is not null then cor.coordsigla 
					     when s.cnvid is not null then cor2.coordsigla 
						 when s.ocuid is not null then cor3.coordsigla 
						 when s.oatid is not null then cor4.coordsigla end as secretaria,
					lcoano, 
					s.sifprofmagisterio,
					(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as valortotal,
					coalesce(s.sifvalorempenhado,0) as valorempenhado,
					'<input type=\"text\" style=\"text-align:right;\" name=\"lcovalor['||lcoid||']\" size=\"15\" maxlength=\"15\" value=\"'||case when lcovalor=0 then '0,00' else trim(coalesce(to_char(lcovalor,'999g999g999d99'),'')) end||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" title=\"Planejamento 2015\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" class=\" normal\">' as aa,
					case when lcoaceito is null then '<span style=font-size:x-small;><center><b>Não respondido</b></center></span>'
						 when lcoaceito = true  then '<span style=font-size:x-small;><center><img src=../imagens/valida1.gif> <b>Aceito</b></center></span>'
						 when lcoaceito = false then '<span style=font-size:x-small;><center><img src=../imagens/valida3.gif> <b>Recusado</b></center></span>'
					 end as botao  
				from sisfor.planejamento2015 p 
				inner join public.unidade u on u.unicod = p.unicod 
				inner join sisfor.sisfor s on s.sifid = p.sifid 
				inner join workflow.documento d on d.docid = s.docidprojeto 
				left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
				left join catalogocurso2014.curso cur on cur.curid = ieo.curid
				left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
				left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
				left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
				left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
				left join seguranca.usuario usu on usu.usucpf = s.usucpf
				left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
				left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
				left join sisfor.outraatividade oat on oat.oatid = s.oatid 
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
				where p.lcoano='2014' and d.esdid='".ESD_PROJETO_VALIDADO."' ".(($wh)?"and ".implode(" and ", $wh):"")."
				order by 1,2
														    		
				) foo";

			
		$cabecalho = array("Cód. UO","IES","Curso validado","Secretaria","Ano","Vagas","Valor aprovado pelo MEC 2014","Valor não empenhado 2014","Valor autorizado pelo MEC 2015");
		$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true);
		
		
		$sql = "select
				'<span style=font-size:x-small;>'||foo.unicod||'</span>' as unicod,
				'<span style=font-size:x-small;>'||foo.universidade||'</span>' as universidade,
				'<span style=font-size:x-small;>'||foo.lconome||'</span>' as lconome,
				'<span style=font-size:x-small;>'||foo.lcosecretaria||'</span>' as lcosecretaria,
				'<span style=font-size:x-small;>'||foo.lcoano||'</span>' as lcoano,
				'<input type=\"text\" style=\"text-align:right;\" name=\"lcovalor['||lcoid||']\" size=\"15\" maxlength=\"15\" value=\"'||case when lcovalor=0 then '0,00' else trim(coalesce(to_char(lcovalor,'999g999g999d99'),'')) end||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" title=\"Planejamento 2015\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" class=\" normal\">' as aa
				from (
				select
					u.unicod, 
					uniabrev||' - '||unidsc as universidade,
					lconome,
					lcoano,
					lcosecretaria,
					lcoid,
					p.lcovalor,
					case when lcoaceito is null then '<span style=font-size:x-small;><center><b>Não respondido</b></center></span>'
						 when lcoaceito = true  then '<span style=font-size:x-small;><center><img src=../imagens/valida1.gif> <b>Aceito</b></center></span>'
						 when lcoaceito = false then '<span style=font-size:x-small;><center><img src=../imagens/valida3.gif> <b>Recusado</b></center></span>'
					 end as botao
				from sisfor.planejamento2015 p
				inner join public.unidade u on u.unicod = p.unicod
		 		where p.lcoano='2015' ".(($wh2)?"and ".implode(" and ", $wh2):"")."
				order by lconome
				) foo";
		
			
		$cabecalho = array("Cód. UO","IES","Curso para oferta 2015","Secretaria","Ano","Valor aprovado pelo MEC 2015");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
		
		?></td>
	</tr>
	<tr>
		<td colspan="2">
		<input type="submit" value="Salvar">
		</td>
	</tr>
</table>
</form>