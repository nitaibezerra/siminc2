<?

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']);
} 

require_once APPRAIZ . "includes/workflow.php";

$sisfor = $db->carregar("select * from sisfor.sisfor where siftipoplanejamento=3 and docidprojeto is null");

if($sisfor[0]) {
	foreach($sisfor as $sif) {
		$docidprojeto = wf_cadastrarDocumento(WF_TPDID_PROJETO,"Projeto do curso ## e universidade {$sif['unicod']}");
		$db->executar("UPDATE sisfor.sisfor SET docidprojeto='".$docidprojeto."' WHERE sifid='".$sif['sifid']."'");
		$db->commit();
	}
}


monta_titulo('Programação 2015',$cies);

?>
<script>


function aprovarCurso(lcoid, obj) {
	var conf = confirm('Deseja realmente ACEITAR o valor proposta pelo MEC?');
	if(conf) {
		ajaxatualizar('requisicao=definirCurso&noredirect=1&lcoaceito=1&lcoid='+lcoid,'');
		obj.parentNode.innerHTML = '<img src=../imagens/valida1.gif> <b>Aceito</b>';
	}
}


function exibirJustificativa(lcoid) {
	jQuery('#lcoid').val(lcoid);
	jQuery("#exibirJustificativa").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function exibirJustificativa2(lcoid) {
	ajaxatualizar('requisicao=exibirJustificativaPlanejamento2015&lcoid='+lcoid,'exibirJustificativa2');
	jQuery("#exibirJustificativa2").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}


function recusarCurso() {

	if(jQuery('#lcoaceitojustificativa').val()=='') {
		alert('Justificativa em branco');
		return false;
	}


	var conf = confirm('Deseja realmente RECUSAR o valor proposta pelo MEC?');
	if(conf) {
		jQuery('#formulario2').submit();
	}
}


function gerenciarCoordenadorCurso(p){
	window.open('sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A'+p,'Coordenador Curso','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
}



</script>

<div id="exibirJustificativa2" style="display:none;" title="Justificativa da recusa"></div>


<div id="exibirJustificativa" style="display:none;">
<form method="post" id="formulario2" name="formulario2">
<input type="hidden" name="lcoid" id="lcoid">
<input type="hidden" name="lcoaceito" id="lcoaceito" value="0">
<input type="hidden" name="requisicao" id="requisicao" value="definirCurso">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Justificativa para recusar</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"></td>
	<td><? echo campo_textarea( 'lcoaceitojustificativa', 'S', 'S', '', '70', '4', '1000'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="recusarcurso" value="Recusar curso" onclick="recusarCurso();"></td>
</tr>
</table>
</form>
</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
	<tr>
		<td class="subtituloDireita" width="25%">Orientações:</td>
		<td>
		<p>Prezados(as) Coordenadores(as) Institucionais,</p>
		<p>Para que possamos formalizar a execução dos recursos orçamentários 2015, definidos por IES, abaixo, encontra-se a  aba "Programação 2015", contendo todos os recursos disponíveis para esse exercício vinculados aos respectivos projetos.</p>
		<p>Informamos que os recursos referentes aos cursos serão liberados mediante envio de NL da SPO do MEC a cada IES, específico por curso. A IES não está autorizada a empenhar valores distintos dos aprovados, e o recurso só poderá ser empenhado para o curso autorizado.</p> 
		</td>
	</tr>
	<tr>
		<td colspan="2">
		
		<div id="resumoPlanejamento2015"><? resumoPlanejamento2015(array('unicod' => $_SESSION['sisfor']['unicod'])); ?></div>
		
		
		<?
		
		
		$sql = "select 
				foo.lconome,
				foo.secretaria,
				foo.lcoano,
				foo.sifprofmagisterio,
		 		'<span style=float:right;>'||case when (foo.valortotal) between 0 AND 1 then '0' else '' end||trim(coalesce(to_char(foo.valortotal,'999g999g999d99'),''))||'<span>' as valortotal,
				'<span style=float:right;>'||case when (foo.valortotal-foo.sifvalorempenhado) between 0 AND 1 then '0' else '' end||trim(coalesce(to_char((foo.valortotal-foo.sifvalorempenhado),'999g999g999d99'),''))||'<span>' as valornaoempenhado,
				'<span style=float:right;>'||case when (foo.lcovalor) between 0 AND 1 then '0' else '' end||trim(coalesce(to_char(lcovalor,'999g999g999d99'),'<b>Não informado</b>'))||'</span>' as lcovalor
				from (
				select 
					lconome, 
					lcoano, 
					s.sifprofmagisterio,
					(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as valortotal,
					s.sifvalorempenhado,
					p.lcovalor,
					case when s.ieoid is not null then cor.coordsigla 
					     when s.cnvid is not null then cor2.coordsigla 
					     when s.ocuid is not null then cor3.coordsigla 
					     when s.oatid is not null then cor4.coordsigla end as secretaria,
					case when lcoaceito is null then '<center><input type=button name=aceitar value=\"Aceitar\" onclick=\"aprovarCurso('||p.lcoid||', this);\"> <input type=button name=recusar value=\"Recusar\" onclick=\"exibirJustificativa('||p.lcoid||');\"></center>'
						 when lcoaceito = true  then '<center><img src=../imagens/valida1.gif> <b>Aceito</b></center>'
						 when lcoaceito = false then '<center><span style=\"cursor:pointer;\" onclick=\"exibirJustificativa2('||p.lcoid||')\"><img src=../imagens/valida3.gif> <b>Recusado</b></span></center>'
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
		 		where p.unicod='".$_SESSION['sisfor']['unicod']."' and p.lcoano='2014' and d.esdid='".ESD_PROJETO_VALIDADO."' 
				order by lconome
				) foo";

			
		$cabecalho = array("Curso validado 2014 - em andamento","Secretaria","Ano","Vagas","Valor aprovado pelo MEC 2014 ","Valor não empenhado 2014","Valor autorizado pelo MEC 2015","&nbsp;");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
		
		echo '<br><br>';
		
		$sql = "select 
				'<img border=\"0\" title=\"Coordenador de curso\" src=\"../imagens/usuario.gif\" style=\"cursor:pointer;\" onclick=\"gerenciarCoordenadorCurso(\''||foo.p||'\')\">' as acao,
				foo.lconome,
				foo.lcosecretaria,
				foo.lcoano,
				'<span style=float:right;>'||trim(coalesce(to_char(lcovalor,'999g999g999d99'),''))||'</span>' as lcovalor
				from (
				select
					case when s.ieoid is not null then '&ieoid='||s.ieoid||'&unicod='||s.unicod
						 when s.ocuid is not null then '&ocuid='||s.ocuid||'&unicod='||s.unicod
					end as p,
					lconome,
					lcoano,
					s.sifprofmagisterio,
					(select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A') as valortotal,
					s.sifvalorempenhado,
					p.lcovalor,
					p.lcosecretaria,
					case when lcoaceito is null then '<center><input type=button name=aceitar value=\"Aceitar\" onclick=\"aprovarCurso('||p.lcoid||', this);\"> <input type=button name=recusar value=\"Recusar\" onclick=\"exibirJustificativa('||p.lcoid||');\"></center>'
						 when lcoaceito = true  then '<center><img src=../imagens/valida1.gif> <b>Aceito</b></center>'
						 when lcoaceito = false then '<center><img src=../imagens/valida3.gif> <b>Recusado</b></center>'
					 end as botao
				from sisfor.planejamento2015 p
				inner join public.unidade u on u.unicod = p.unicod
				left join sisfor.sisfor s on s.sifid = p.sifid
		 		where p.unicod='".$_SESSION['sisfor']['unicod']."' and p.lcoano='2015' 
				order by lconome
				) foo";
			
		$cabecalho = array("&nbsp;","Curso para oferta 2015","Secretaria","Ano","Valor autorizado pelo MEC 2015");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
		
		
		?></td>
	</tr>


</table>