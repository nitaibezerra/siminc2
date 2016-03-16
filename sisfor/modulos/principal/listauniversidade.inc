<?php
$perfil = pegarPerfil($_SESSION['usucpf']);

if($_REQUEST['requisicao'] == 'pesquisarUniversidade'){
	header('Content-Type: text/html; charset=iso-8859-1');
	pesquisarUniversidade($perfil,$_POST);
	exit();
}

if($_REQUEST['requisicao'] == 'atualizarMunicipio'){
	header('Content-Type: text/html; charset=iso-8859-1');
	atualizarMunicipio($_POST);
	exit();
}

if($_REQUEST['requisicao']=='verProjetoCurso') {

	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';


	if($_REQUEST['sifid']) $_SESSION['sisfor']['sifid'] = $_REQUEST['sifid'];

	$arr = $db->pegaLinha("SELECT s.unicod,
								  CASE WHEN s.ieoid IS NOT NULL THEN ieo.curid
									   WHEN s.cnvid IS NOT NULL THEN cnv.curid END as curid,
								  s.docidprojeto
						   FROM sisfor.sisfor s
						   LEFT JOIN catalogocurso2014.iesofertante ieo ON ieo.ieoid = s.ieoid
						   LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
						   WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");

	$_SESSION['sisfor']['unicod']       = $arr['unicod'];
	$_SESSION['sisfor']['docidprojeto'] = $arr['docidprojeto'];

	if($arr['curid']) $_SESSION['sisfor']['curid'] = $arr['curid'];
	else unset($_SESSION['sisfor']['curid']);

	include APPRAIZ ."includes/workflow.php";
	include_once APPRAIZ_SISFOR.'coordenador_curso/visualizacao_projeto.inc';

	exit();

}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

if(in_array(PFL_COORDENADOR_INST,$perfil)){
	$titulo_modulo = "Lista Universidade - Coordenador Institucional";
} elseif(in_array(PFL_COORDENADOR_CURSO,$perfil)) {
	$titulo_modulo = "Lista Universidade - Coordenador Curso";
} else if(in_array(PFL_EQUIPE_MEC,$perfil)){
	$titulo_modulo = "Lista Universidade - Equipe MEC";	
} else {
	$titulo_modulo = "Lista Universidade";	
}

monta_titulo( $titulo_modulo, $modulo);

if($_SESSION['sisfor']['unicod']){
	unset($_SESSION['sisfor']['unicod']);
}

if($_SESSION['sisfor']['curid']){
	unset($_SESSION['sisfor']['curid']);
}
?>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>
<script language="javascript" type="text/javascript">
	jQuery.noConflict();
	
	jQuery(document).ready( function (){
		<?php if(in_array(PFL_COORDENADOR_CURSO,$perfil)){ ?>
		jQuery('[id^="img_dimensao_"]').each( function(){
			idsplit = (this.id).split('img_dimensao_');
			carregarListaUniversidade(this.id, idsplit[1]); 
		});
		<?php } ?>
	});

	function pesquisarUniversidade(){
		divCarregando();
		jQuery.ajax({
		type: 'POST',
		url: 'sisfor.php?modulo=principal/listauniversidade&acao=A',
		data: jQuery('#formulario').serialize(),
		async: false,
		success: function(data) {
				jQuery("#div_universidade").html(data);
				divCarregado();
	    	}
		});
	}

	function acessarCurso(curid, unicod) {
		window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&curid='+curid+'&unicod='+unicod;

	}
	
	function atualizarMunicipio(estuf){
		divCarregando();
		jQuery.ajax({
			type: 'POST',
			url: 'sisfor.php?modulo=principal/listauniversidade&acao=A',
			data: { requisicao:'atualizarMunicipio', estuf: estuf},
			async: false,
			success: function(data){
				jQuery("#div_municipio").html(data);
				divCarregado();
		    }
		});
	}
	
	function gerenciarCoordenadorIES(unicod) {
		window.open('sisfor.php?modulo=principal/coordenador/gerenciarcoordenador_ies&acao=A&unicod='+unicod,'Coordenador IES','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}

	function alterarCoordenadorIES(iusd) {
		window.location='sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&iusd='+iusd;
	}

	function ofertar2015(iusd) {
		window.location='sisfor.php?modulo=principal/coordenador/oferta2015&acao=A&iusd='+iusd;
	}
	
	function selecionarBolsa(){
		window.location='sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&iusd='+iusd;
	}

	function acessarCursoDireto(sifid) {
		window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&sifid='+sifid;
	}

	function acessarOutrosPerfis(url) {
		window.location=url;
	}



	function verProjetoCurso(sifid) {

		ajaxatualizar('requisicao=verProjetoCurso&sifid='+sifid,'modalDiv');

		jQuery("#modalDiv").dialog({
	        draggable:true,
	        resizable:true,
	        width: 1280,
	        height: 600,
	        modal: true,
	     	close: function(){} 
	    });

		
	}

	function wf_exibirHistorico( docid )
	{
		var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}
	
</script>

<div id="modalDiv" style="display: none;"></div>

<?php if(in_array(PFL_SUPER_USUARIO,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil) || in_array(PFL_EQUIPE_MEC,$perfil) || in_array(PFL_CONSULTAGERAL,$perfil)){ ?>

<form id="formulario" method="post" name="formulario" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="pesquisarUniversidade" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Código da Unidade Orçamentária:</td>
			<td><?php echo campo_texto('unicod', 'N', 'S', 'Código da Unidade Orçamentária', '50', '5', '[#]', '','',''); ?></td>
		</tr>				
		<tr>
			<td class="subtituloDireita">Sigla da Instituição:</td>
			<td><?php echo campo_texto('uniabrev', 'N', 'S', 'Sigla da Instituição', '50', '', '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Coordenador Institucional:</td>
			<td><?php echo campo_texto('iusnome', 'N', 'S', 'Coordenador Institucional', '50', '', '', '','',''); ?></td>
		</tr>			
		<tr>
			<td class="subtituloDireita">CPF Coordenador Institucional:</td>
			<td><?php echo campo_texto('iuscpf', 'N', 'S', 'CPF Coordenador Institucional', '50', '14', '###.###.###-##', '','',''); ?></td>
		</tr>					
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarUniversidade();"/>
			</td>
		</tr>
	</table>
</form>
<?php } ?>

<br>

<?php if(in_array(PFL_SUPER_USUARIO,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil) || in_array(PFL_EQUIPE_MEC,$perfil) || in_array(PFL_COORDENADOR_INST,$perfil) || in_array(PFL_CONSULTAGERAL,$perfil)){ ?>
<div id="div_universidade"><?php pesquisarUniversidade($perfil); ?></div>
<?php } ?>

<?php 

if(in_array(PFL_COORDENADOR_CURSO,$perfil) || in_array(PFL_COORDENADOR_ADJUNTO_IES,$perfil)) :
	
	monta_titulo( "Lista de cursos - Coordenador Geral/Adjunto", "&nbsp;");
	
	$acao = "'<span style=white-space:nowrap;><img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"verProjetoCurso('||s.sifid||');\"> <img style=\"cursor: pointer;\" src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''||s.docidprojeto||'\' );\"> <img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||s.sifid||')\"></span>'";
	
	$sql = "select
			{$acao} as acao,
			'<span style=font-size:x-small;>'||COALESCE(usu.usunome,'Não cadastrado')||'</span>' as usunome,
			'<span style=font-size:x-small;>'||uniabrev||' - '||unidsc||'</span>' as universidade,
			'<span style=font-size:x-small;>'||case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
													when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
													when s.ocuid is not null then oc.ocunome
													when s.oatid is not null then oatnome end||'</span>' as curso,
			'<span style=font-size:x-small;'||case when e.esdid!=".ESD_PROJETO_VALIDADO." then 'color:red;' else '' end||'><b>'||e.esddsc||'</b></span>' as esddsc,
			'<span style=font-size:x-small;>'||coalesce(ectur.esddsc,'Não iniciou')||'</span>' as esddsc_comporturma,
			'<span style=font-size:x-small;>'||case when s.ieoid is not null then cor.coordsigla
													when s.cnvid is not null then cor2.coordsigla
													when s.ocuid is not null then cor3.coordsigla
													when s.oatid is not null then cor4.coordsigla end||'</span>' as secretaria,
			'<span style=font-size:x-small;>'||case when s.ieoid is not null then case when s.sifopcao='1' then 'Curso Proposto pelo MEC – Aceito'
																						when s.sifopcao='2' then 'Curso Proposto pelo MEC – Rejeitado'
																						when s.sifopcao='3' then 'Curso Proposto pelo MEC – Repactuado'
																						else 'não definido' end
																						when s.cnvid is not null then 'Curso Proposto pela IES – Do Catálogo'
																						when s.ocuid is not null then 'Curso Proposto pela IES – Fora do Catálogo'
																						when s.oatid is not null then 'Outras Atividades' end||'</span>' as tipo
			from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
			left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid
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
			left join (
			select t.sifid, i.iuscpf from sisfor.identificacaousuario i 
			inner join sisfor.tipoperfil t on t.iusd = i.iusd 
			where i.iusstatus='A' and t.pflcod=".PFL_COORDENADOR_ADJUNTO_IES."
			) cadjunto on cadjunto.sifid = s.sifid and cadjunto.iuscpf='".$_SESSION['usucpf']."'
			left join (
			select t.sifid, i.iuscpf from sisfor.identificacaousuario i 
			inner join sisfor.tipoperfil t on t.iusd = i.iusd 
			where i.iusstatus='A' and t.pflcod=".PFL_COORDENADOR_CURSO."
			) cgeral on cgeral.sifid = s.sifid and cgeral.iuscpf='".$_SESSION['usucpf']."'
			where sifstatus='A' and (cgeral.iuscpf='".$_SESSION['usucpf']."' or cadjunto.iuscpf='".$_SESSION['usucpf']."' or s.usucpf='".$_SESSION['usucpf']."')
			order by unidsc";
	
	$cabecalho = array("&nbsp;","Coordenador do curso","Universidade","Curso","Situação","Equipe","Secretaria","Tipo de curso");
	
	$param['managerOrder'][8]['campo'] = 'htddata';
	$param['managerOrder'][8]['alias'] = 'htddata';
	
	
	$db->monta_lista($sql,$cabecalho,50,10,'N','center','','','','','',$param);
	
endif; 

if(in_array(PFL_SUPERVISOR_IES,$perfil) || in_array(PFL_FORMADOR_IES,$perfil) || in_array(PFL_PROFESSOR_PESQUISADOR,$perfil)) :

	monta_titulo( "Lista de cursos - Outros perfil", "&nbsp;");
		
	$acao = "'<img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarOutrosPerfis(\''||
			case 
				when t.pflcod='".PFL_FORMADOR_IES."' then 'sisfor.php?modulo=principal/formador/formador&acao=A'
	        	when t.pflcod='".PFL_PROFESSOR_PESQUISADOR."' then 'sisfor.php?modulo=principal/professor_pesquisador/professor_pesquisador&acao=A' 
				when t.pflcod='".PFL_SUPERVISOR_IES."' then 'sisfor.php?modulo=principal/supervisor/supervisor&acao=A'
			end
			||'\');\"></span>'";
		
	$sql = "select
		{$acao} as acao,
		'<span style=font-size:x-small;>'||COALESCE(i.iusnome,'Não cadastrado')||'</span>' as usunome,
		'<span style=font-size:x-small;>'||UPPER(p.pfldsc)||'</span>' as perfil,
		'<span style=font-size:x-small;>'||uniabrev||' - '||unidsc||'</span>' as universidade,
		'<span style=font-size:x-small;>'||case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
		when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
		when s.ocuid is not null then oc.ocunome
		when s.oatid is not null then oatnome end||'</span>' as curso,
		'<span style=font-size:x-small;'||case when e.esdid!=".ESD_PROJETO_VALIDADO." then 'color:red;' else '' end||'><b>'||e.esddsc||'</b></span>' as esddsc,
					'<span style=font-size:x-small;>'||case when s.ieoid is not null then cor.coordsigla
															when s.cnvid is not null then cor2.coordsigla
															when s.ocuid is not null then cor3.coordsigla
															when s.oatid is not null then cor4.coordsigla end||'</span>' as secretaria,
					'<span style=font-size:x-small;>'||case when s.ieoid is not null then case when s.sifopcao='1' then 'Curso Proposto pelo MEC – Aceito'
																								when s.sifopcao='2' then 'Curso Proposto pelo MEC – Rejeitado'
																								when s.sifopcao='3' then 'Curso Proposto pelo MEC – Repactuado'
																								else 'não definido' end
																								when s.cnvid is not null then 'Curso Proposto pela IES – Do Catálogo'
																								when s.ocuid is not null then 'Curso Proposto pela IES – Fora do Catálogo'
																								when s.oatid is not null then 'Outras Atividades' end||'</span>' as tipo
					from sisfor.sisfor s 
					inner join sisfor.tipoperfil t on t.sifid = s.sifid 
					inner join seguranca.perfil p on p.pflcod = t.pflcod 
					inner join sisfor.identificacaousuario i on i.iusd = t.iusd 
					inner join workflow.documento d on d.docid = s.docidprojeto
					left join workflow.historicodocumento h on h.hstid = d.hstid
					inner join workflow.estadodocumento e on e.esdid = d.esdid
					inner join public.unidade u on u.unicod = s.unicod
					left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
					left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid
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
					where sifstatus='A' and i.iuscpf='".$_SESSION['usucpf']."' and t.pflcod in(".PFL_FORMADOR_IES.",".PFL_PROFESSOR_PESQUISADOR.",".PFL_SUPERVISOR_IES.")
					order by unidsc";

	$cabecalho = array("&nbsp;","Nome","Perfil","Universidade","Curso","Situação","Secretaria","Tipo de curso");

	$param['managerOrder'][8]['campo'] = 'htddata';
	$param['managerOrder'][8]['alias'] = 'htddata';

	$db->monta_lista($sql,$cabecalho,50,10,'N','center','','','','','',$param);

endif;


?>