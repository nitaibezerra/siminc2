<?php 
include_once APPRAIZ . "includes/workflow.php";

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']);
}
	
$planejamento = carregarDadosPlanejamento();
extract($planejamento);

if($_SESSION['sisfor']['unicod'] && empty($docidplan2)){
	$docidplan2 = criarDocumentoPlanejamento2($_SESSION['sisfor']['unicod']);
	$esdidplan2 = pegarEstadoDocumento($docidplan2);
} 

if($_SESSION['usucpf']){
	$perfil = pegarPerfil($_SESSION['usucpf']);
}

monta_titulo('Planejamento - Fase 2',$cies); ?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script language="javascript" type="text/javascript">
	function selecionarCurso(tcurso,sifid){
		if(tcurso == 'S'){
			return window.open('sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo2&acao=A','Cadastro Curso Catálogo - Fase 2',"height=450,width=850,scrollbars=yes,top=50,left=200");
		} else if(tcurso == 'N'){
			return window.open('sisfor.php?modulo=principal/coordenador/cadastrooutroscursos2&acao=A','Cadastro Outros Cursos - Fase 2',"height=450,width=850,scrollbars=yes,top=50,left=200");
		} else if(tcurso == 'AD'){
			return window.open('sisfor.php?modulo=principal/coordenador/cadastrocursoaditivo&acao=A','Cadastro Curso Aditivo - Fase 2',"height=450,width=850,scrollbars=yes,top=50,left=200");
		} else if(tcurso == 'A'){
			return window.open('sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A','Cadastro Outras Atividades - Fase 2',"height=450,width=850,scrollbars=yes,top=50,left=200");
		}
	}

	function selecionarAtividade(oatid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A&oatid='+oatid,'Cadastro Outras Atividades',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}

	function selecionarCursoNVinculado(cnvid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo2&acao=A&cnvid='+cnvid,'Cadastro Curso Catálogo',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}		

	function selecionarOutroCurso(ocuid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastrooutroscursos2&acao=A&ocuid='+ocuid,'Cadastro Outros Cursos',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}	

	function adicionarPropostaIES(ieoid,unicod){
		return window.open('sisfor.php?modulo=principal/coordenador/propostaies&acao=A&ieoid='+ieoid+'&unicod='+unicod,'Proposta IES',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}

	function alterarPropostaIES(sifid){
		return window.open('sisfor.php?modulo=principal/coordenador/propostaies&acao=A&sifid='+sifid,'Proposta IES',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}	

	function gerenciarCoordenadorCursoIEOID(unicod,ieoid){
		window.open('sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&ieoid='+ieoid+'&unicod='+unicod,'Coordenador Curso','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}

	function gerenciarCoordenadorCursoCNVID(unicod,cnvid){
		window.open('sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&cnvid='+cnvid+'&unicod='+unicod,'Coordenador Curso','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}

	function gerenciarCoordenadorCursoOCUID(unicod,ocuid){
		window.open('sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&ocuid='+ocuid+'&unicod='+unicod,'Coordenador Curso','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}	

	function gerenciarCoordenadorAtividadeOATID(unicod,oatid) {
		window.open('sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&oatid='+oatid+'&unicod='+unicod,'Coordenador Atividade','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}	
	
	function imprimirCurso(curid){
		window.open('sisfor.php?modulo=principal/coordenador/impressaocurso&acao=A&curid='+curid,'Visualizar Curso','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}		

	function imprimirCursoOCUID(ocuid){
		window.open('sisfor.php?modulo=principal/coordenador/impressaocurso&acao=A&ocuid='+ocuid,'Visualizar Curso','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}		

	function imprimirCursoCNVID(cnvid){
		window.open('sisfor.php?modulo=principal/coordenador/impressaocurso&acao=A&cnvid='+cnvid,'Visualizar Curso','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}		

	function imprimirCursoOATID(oatid){
		window.open('sisfor.php?modulo=principal/coordenador/impressaocurso&acao=A&oatid='+oatid,'Visualizar Curso','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}				

	function abrirJustificativa(sifid){
		window.open('sisfor.php?modulo=principal/coordenador/justificativa&acao=A&sifid='+sifid,'Visualizar Justificativa','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}	

	function excluirAtividade(oatid){
		if(confirm("Deseja realmente excluir a Atividade? Todas os itens serão excluídos!")){
			divCarregando();
			jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2',
			data: { requisicao: 'excluirAtividade', oatid: oatid, siftipoplanejamento: 2},
			async: false,
			success: function(data) {
					alert('Atividade excluída com sucesso!');
					jQuery("#divListarOutrasAtividades").html(data);
					divCarregado();
		    	}
			});
		}
	}

	function excluirCursoCatalogo(cnvid){
		if(confirm("Deseja realmente excluir o Curso?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2',
				data: { requisicao: 'excluirCursoCatalogo', cnvid: cnvid, siftipoplanejamento: 2},
				async: false,
				success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListarCursosPropostoIES").html(data);
					jQuery('#valor').load('sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2 #valor');
					divCarregado();
			    }
			});
		}
	}    

	function excluirCursoOutros(ocuid){
		if(confirm("Deseja realmente excluir o Curso?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2',
				data: { requisicao: 'excluirCursoOutros', ocuid: ocuid, siftipoplanejamento: 2},
				async: false,
				success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListarCursosPropostoIES").html(data);
					divCarregado();
			    }
			});
		}
	}

	function acessarCurso(curid, unicod) {
		window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&curid='+curid+'&unicod='+unicod;
	}

	function acessarCursoDireto(sifid) {
		window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&sifid='+sifid;
	}

	function inserirCustoAtividade(sifid){
		return window.open('sisfor.php?modulo=principal/coordenador/inserircustosatividade&acao=A&sifid='+sifid,'Solicitação','height=600,width=825,scrollbars=yes,top=5,left=500');
	}

    function carregarListaCustoAtividade(idImg, sifid){
        var img = $( '#'+idImg );
        var tr_nome = 'listaCusto_'+ sifid;
        var td_nome = 'trA_'+ sifid;

        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "" ){
            $('#'+td_nome).html('Carregando...');
            img.attr ('src','../imagens/menos.gif');
            exibirListaCustoAtividade(sifid, td_nome);
        }
        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
            $('#'+tr_nome).css('display','');
            img.attr('src','../imagens/menos.gif');
        } else {
            $('#'+tr_nome).css('display','none');
            img.attr('src','/imagens/mais.gif');
        }
    }

    function exibirListaCustoAtividade(sifid, td_nome){
    	jQuery.ajax({
   	   		type: 'POST',
   	   		url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2',
   	   		data: { requisicao: 'exibirListaCustoAtividade', sifid: sifid},
   	   		async: false,
   	   		success: function(msg){
   	   			jQuery('#'+td_nome).html(msg);
   	   		}
   		});
    }

	function visualizarCustoAtividade(orcid){
		return window.open('sisfor.php?modulo=principal/coordenador/inserircustosatividade&acao=A&orcid='+orcid,'Solicitação','height=600,width=825,scrollbars=yes,top=5,left=500');
	}	

	function excluirCustoAtividade(orcid){
		if(confirm("Deseja realmente excluir o Custo Atividade?")){
			divCarregando();
			jQuery.ajax({
				type: 'POST',
				url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2',
				data: { requisicao: 'excluirCustoAtividade', orcid: orcid},
				async: false,
				success: function(data) {
					alert('Custo Atividade excluído com sucesso!');
					jQuery('#divListarOutrasAtividades').load('sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2 #divListarOutrasAtividades');
					divCarregado();
			    }
			});
		}
	}		    

	function abrirCursoValidado(){
		return window.open('sisfor.php?modulo=principal/coordenador/cursovalidado&acao=A','Cursos Validado','height=600,width=900,scrollbars=yes,top=5,left=500');
	}
	
	function abrirListaCoordenadores(){
		return window.open('sisfor.php?modulo=principal/coordenador/listacoordenadores&acao=A','Coordenadores Validado','height=600,width=1200,scrollbars=yes,top=5,left=500');
	}

	function alterarCursoAditivo(sifid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastrocursoaditivo&acao=A&sifid='+sifid,'Cadastro Curso Aditivo - Fase 2','height=450,width=850,scrollbars=yes,top=5,left=500');
	}

	jQuery(function(){
		jQuery('[id^=img_dimensao_]').click();
	});	
</script>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="90%">Orientações</td>
		<td align="center" rowspan="2">
			<?php if($docidplan2){ 		
				wf_desenhaBarraNavegacao( $docidplan2 , array('unicod' => $_SESSION['sisfor']['unicod']), ''); 
			} ?>		
		</td>
	</tr>
	<tr>
		<td>
			<p align="justify" style="line-height: 17px; font-size: 14px;;">
			A Fase 2 do Planejamento consiste em informar, a partir do saldo de recursos orçamentários ainda não comprometidos com projetos validados pelo MEC, 
			a destinação dos valores remanescentes e ainda passíveis de utilização neste exercício. 
			Sugere-se que seja feita uma articulação prévia com os coordenadores dos cursos no MEC antes da inclusão dos mesmos no Planejamento, 
			evitando inclusão de propostas consideradas incompatíveis com a política de formação continuada ou o encaminhamento para as Diretorias erradas. 
			A lista dos coordenadores dos cursos e os e-mails institucionais estão disponíveis neste <a onclick="abrirListaCoordenadores();">link</a>. 
			</p>
		</td>
	</tr>		
</table>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="25%"></td>
		<td class="subtituloEsquerda" width="80%">Valor</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">LOA</td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor($saldoloa); ?></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%"><a onclick="abrirCursoValidado();">Valor comprometido com projetos Validados na Fase 1</a></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor($saldoprojeto); ?></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">Saldo total para a Fase 2</td>
		<?php if($saldodisp > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo formata_valor($saldodisp); ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo formata_valor($saldodisp); ?></b></font></td>
		<?php } ?>
	</tr>	
	<!-- 
	<tr>
		<td class="subtituloEsquerda">Aditivo Fase 1:</td>
		<?php if($saldoaditivo > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo formata_valor($saldoaditivo); ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo formata_valor($saldoaditivo); ?></b></font></td>
		<?php } ?>		
	</tr>
	 -->			
	<tr>
		<td class="subtituloEsquerda" width="25%">Valor não aplicado em projetos da Fase 2</td>
		<?php if($saldocomp > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo formata_valor($saldocomp); ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo formata_valor($saldocomp); ?></b></font></td>
		<?php } ?>		
	</tr>		
</table>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="10%"><b>CURSOS PROPOSTOS PELA IES - Fase 2</b></td>
	</tr>
</table>

<?php if(!in_array(PFL_EQUIPE_MEC,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil) || in_array(PFL_SUPER_USUARIO,$perfil) || ( in_array(PFL_COORDENADOR_INST,$perfil) && $esdidplan2 <> WF_PLAN_ANALISE_MEC)){ ?>
<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="80%">
	<tr>
		<td class="subtituloDireita" width="10%"><b>Incluir Curso:</b></td>
		<?php if($saldocomp <= 0){ ?>
		<td><font color="red"><b>Não há saldo suficiente de recursos LOA.</b></font></td>	
		<?php } else { ?>
		<td>
			<input id="tcurso" type="radio" name="tcurso" value="S" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Curso do Catálogo MEC
			<input id="tcurso" type="radio" name="tcurso" value="N" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Curso fora do Catálogo MEC
			<input id="tcurso" type="radio" name="tcurso" value="A" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Outras Atividades
			<!-- 
			<input id="tcurso" type="radio" name="tcurso" value="AD" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Aditivo
			 -->
		</td>	
		<?php } ?>		
	</tr>
</table>
<?php } ?>

<div id="divListarCursosPropostoIES">
	<?php 
	validaCoordenadoresFase2();
	listarCursosPropostoIES(array(),FASE02); ?>
</div>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>	
		<td class="subtituloCentro" width="10%"><b>OUTRAS ATIVIDADES - Fase 2</b></td>
	</tr>
</table>

<div id="divListarOutrasAtividades">
	<?php listarAtividadesPropostaIES(FASE02); ?>
</div>

<!-- 
<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>	
		<td class="subtituloCentro" width="10%"><b>CURSOS COM ADITIVO - Fase 1</b></td>
	</tr>
</table>

<div id="divListarCursoAditivo">
	<?php listarCursoAditivo(); ?>
</div>
 -->