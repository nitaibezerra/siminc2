<?php 
include_once APPRAIZ . "includes/workflow.php";

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']);
}
	
$seb = carregarDadosSEB();
$secadi = carregarDadosSECADI();

$saldoseb = verificarSaldoSEB();
$saldosecadi = verificarSaldoSECADI();

if($_SESSION['usucpf']){
	$perfil = pegarPerfil($_SESSION['usucpf']);
}
	
if($esdid == ESD_VALIDADO_MEC){
	$planejamentofechado = true;
} else {
	$planejamentofechado = false;
}

if($_SESSION['sisfor']['unicod'] && empty($docid)){
	$docid = criarDocumentoPlanejamento($_SESSION['sisfor']['unicod']);
	$esdid = pegarEstadoDocumento($docid);
} 

monta_titulo('Planejamento',$cies); ?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script language="javascript" type="text/javascript">
	function selecionarCurso(tcurso,sifid){
		if(tcurso == 'S'){
			return window.open('sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo&acao=A','Cadastro Curso Catálogo',"height=450,width=850,scrollbars=yes,top=50,left=200");
		} else if(tcurso == 'N'){
			return window.open('sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A','Cadastro Outros Cursos',"height=450,width=850,scrollbars=yes,top=50,left=200");
		} else {
			return window.open('sisfor.php?modulo=principal/coordenador/cadastroatividades&acao=A','Cadastro Outras Atividades',"height=450,width=850,scrollbars=yes,top=50,left=200");
		}
	}

	function selecionarAtividade(oatid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastroatividades&acao=A&oatid='+oatid,'Cadastro Outras Atividades',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}

	function selecionarCursoNVinculado(cnvid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo&acao=A&cnvid='+cnvid,'Cadastro Curso Catálogo',"height=450,width=850,scrollbars=yes,top=50,left=200");
	}		

	function selecionarOutroCurso(ocuid){
		return window.open('sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&ocuid='+ocuid,'Cadastro Outros Cursos',"height=450,width=850,scrollbars=yes,top=50,left=200");
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
	
	function abrirWorkflowCurso(sifid){
		window.open('sisfor.php?modulo=principal/coordenador/wfplanejamento&acao=A&sifid='+sifid,'WorkFlow Curso','scrollbars=no,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}

	function abrirJustificativa(sifid){
		window.open('sisfor.php?modulo=principal/coordenador/justificativa&acao=A&sifid='+sifid,'Visualizar Justificativa','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}	

	function excluirAtividade(oatid){
		if(confirm("Deseja realmente excluir a Atividade?")){
			divCarregando();
			jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies',
			data: { requisicao: 'excluirAtividade', oatid: oatid, siftipoplanejamento: 1},
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
				url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies',
				data: { requisicao: 'excluirCursoCatalogo', cnvid: cnvid, siftipoplanejamento: 1},
				async: false,
				success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListarCursosPropostoIES").html(data);
					divCarregado();
			    }
			});
		}
	}    

	function excluirCursoOutros(ocuid){
		if(confirm("Deseja realmente excluir o Curso?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies',
				data: { requisicao: 'excluirCursoOutros', ocuid: ocuid, siftipoplanejamento: 1},
				async: false,
				success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListarCursosPropostoIES").html(data);
					divCarregado();
			    }
			});
		}
	}

	function excluirCursoMEC(sifid){
		if(confirm("Deseja realmente excluir o Curso?")){
			divCarregando();
			jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies',
			data: { requisicao: 'excluirCursoMEC', sifid: sifid},
			async: false,
			success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListarCursoMEC").html(data);
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
</script>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="90%">Orientações</td>
		<td align="center" rowspan="2">
			<?php if($docid){ 		
				wf_desenhaBarraNavegacao( $docid , array('unicod' => $_SESSION['sisfor']['unicod']), ''); 
			} ?>		
		</td>
	</tr>
	<tr>
		<td>
			<p align="justify" style="line-height: 17px;">
				Esta tela exibe três conjuntos de informações: o primeiro é uma síntese dos dados informados nos outros dois campos, indicando os valores previstos pelo MEC para realização dos cursos com recursos orçamentários (para cada Secretaria e Total); abaixo, os valores propostos pela Instituição; e, por fim, os valores relativos a outros cursos e atividades propostos pela IES (incluindo recursos orçamentários ou outros disponíveis nas universidades, 
				inclusive recursos de outros exercícios transferidos às fundações para a mesma finalidade).
			</p>
			<p align="justify" style="line-height: 17px;">
				O segundo campo é o mais importante desta tela, pois indica o conjunto de cursos que o MEC propõe que sejam realizados pela Instituição em 2014 com os recursos orçamentários e botões de ação para receber o posicionamento da IES. Em alguns casos, constam também os valores de cursos aprovados em 2013 e que deverão utilizar recursos da matriz orçamentária 2014 (estes últimos não podem ser alterados, pois trata-se de projetos já aprovados).
			</p>
			<p align="justify" style="line-height: 17px;">
				Para cada curso, o Coordenador Institucional deverá clicar no ícone <img border="0" style="cursor:pointer;" src="../imagens/alterar.gif" title="PROPOSTA IES"> e informar, na janela aberta, se "Aceita", "Rejeita" ou "Repactua" a proposta do MEC. Caso "aceite", o sistema replica o número de vagas e valores sugeridos, impedindo a edição. Caso a IES "rejeite", o sistema indica zero nas vagas e valores, impedindo a edição e abrindo um campo 
				de Justificativa, onde deverá ser descrito, de forma sucinta, o motivo da recusa. Caso "repactue", o sistema deixa os campos de vaga e valor em branco e também solicita uma Justificativa sobre o porquê da alteração sobre a proposta (nas vagas, nos valores ou em ambos, se for o caso). Em todos os casos, não esqueça de clicar em "Salvar".
			</p>		
			<p align="justify" style="line-height: 17px;">
				O Coordenador Institucional também deverá indicar o nome do Coordenador de cada curso clicando no ícone <img border="0" style="cursor:pointer;" src="../imagens/usuario.gif" title="COORDENADOR CURSO">  e informando, na janela aberta, o CPF e o e-mail do responsável (o SisFor fará a validação do nome conforme consta no cadastro de CPFs da Receita Federal). Não esqueça de verificar se a opção "Ativar coordenador no SisFor" está selecionada. Caso deseje 
				substituir um Coordenador de curso, clique primeiro em "Remover Coordenador do curso" e confirme. Em seguida, cadastre o coordenador atual repetindo os procedimentos iniciais. Para os usuários que nunca utilizaram o SIMEC, a senha inicial é “simecdti”. Para quem já acessa, a senha de acesso é a mesma.
			</p>
			<p align="justify" style="line-height: 17px;">
				O terceiro quadro permite à Instituição propor ao MEC a realização de outros cursos e atividades relacionadas à formação de profissionais do magistério. Para acrescentar uma proposta de curso, indique se é um curso do Catálogo do MEC ou fora do Catálogo. No primeiro caso, selecione o curso que a IES deseja ofertar, indique o número de vagas pretendidas e o valor estimado (indicando no campo correspondente o valor proveniente da LOA 2014 e a parte 
				que será realizada com recursos da IES) e clique em "Salvar". Os cursos inseridos vão aparecendo na linha inferior. Para excluir um curso cadastrado, clique no ícone <img border="0" style="cursor:pointer;" src="../imagens/excluir.gif" title="EXCLUIR CURSO">. Para saber mais informações sobre determinado curso do Catálogo do MEC, clique no ícone <img border="0" style="cursor:pointer;" src="../imagens/print.gif" title="IMPRESSÃO CURSO">.
			</p>		
			<p align="justify" style="line-height: 17px;">	 
				No caso dos cursos fora do Catálogo, na janela de cadastro, selecione a Secretaria do MEC e a Diretoria para a qual a proposta será submetida. Preencha os demais campos, incluindo uma breve descrição do curso e anexe um documento com a descrição detalhada (este documento é obrigatório). Ao final, clique em "Salvar".
			</p>
			<p align="justify" style="line-height: 17px;">	 
				Por fim, no caso de outras atividades ligadas à formação de profissionais do magistério, clique em "Outras atividades". Neste caso, indique apenas a Secretaria interessada, escolha a atividade e informe o valor dos recursos orçamentários a serem utilizados. Se as atividades forem realizadas integralmente com recursos da Instituição, não é preciso cadastra-las no SisFor.
			</p>			
			<p align="justify" style="line-height: 17px;">	 	
				Para maiores informações, clique aqui e acesse o tutorial do SisFor. Em caso de dúvidas, envie um e-mail para <?php echo $_SESSION['email_sistema']; ?>. 
			</p>			
		</td>
	</tr>		
</table>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="25%">Proposta MEC</td>
		<td class="subtituloEsquerda" width="25%">SEB</td>
		<td class="subtituloEsquerda" width="25%">SECADI</td>
		<td class="subtituloEsquerda" width="25%">TOTAL</td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">LOA</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valorloa']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valorloa']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valorloa']) + desformata_valor($secadi['valorloa'])); ?></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">Proposta MEC</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valorestimado']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valorestimado']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valorestimado']) + desformata_valor($secadi['valorestimado'])); ?></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">Saldo LOA (Proposta MEC)</td>
		<?php if($seb['saldomec'] > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo $seb['saldomec']; ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo $seb['saldomec']; ?></b></font></td>
		<?php } ?>
		
		<?php if($secadi['saldomec'] > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo $secadi['saldomec']; ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo $secadi['saldomec']; ?></b></font></td>
		<?php } ?>
		
		<?php if((desformata_valor($seb['saldomec']) + desformata_valor($secadi['saldomec'])) > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo formata_valor(desformata_valor($seb['saldomec']) + desformata_valor($secadi['saldomec'])); ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo formata_valor(desformata_valor($seb['saldomec']) + desformata_valor($secadi['saldomec'])); ?></b></font></td>
		<?php } ?>						
	</tr>		
	<tr>
		<td colspan="2" height="10"></td>
	</tr>
	<tr>
		<td class="subtituloCentro" width="25%">Proposta IES</td>
		<td class="subtituloEsquerda" width="25%">SEB</td>
		<td class="subtituloEsquerda" width="25%">SECADI</td>
		<td class="subtituloEsquerda" width="25%">TOTAL</td>
	</tr>	
	<tr>
		<td class="subtituloEsquerda" width="25%">LOA</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valorloa']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valorloa']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valorloa']) + desformata_valor($secadi['valorloa'])); ?></td>
	</tr>	
	<tr>
		<td class="subtituloEsquerda" width="25%">Proposta IES</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valormec']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valormec']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valormec']) + desformata_valor($secadi['valormec'])); ?></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">Saldo LOA (Proposta IES)</td>
		<?php if($seb['saldoies'] > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo $seb['saldoies']; ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo $seb['saldoies']; ?></b></font></td>
		<?php } ?>		
		
		<?php if($secadi['saldoies'] > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo $secadi['saldoies']; ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo $secadi['saldoies']; ?></b></font></td>
		<?php } ?>		
		
		<?php if((desformata_valor($seb['saldoies']) + desformata_valor($secadi['saldoies'])) > 0){ ?>
		<td bgcolor="#e9e9e9"><font color="blue"><b>R$ <?php echo formata_valor(desformata_valor($seb['saldoies']) + desformata_valor($secadi['saldoies'])); ?></b></font></td>
		<?php } else { ?>
		<td bgcolor="#e9e9e9"><font color="red"><b>R$ <?php echo formata_valor(desformata_valor($seb['saldoies']) + desformata_valor($secadi['saldoies'])); ?></b></font></td>
		<?php } ?>				
	</tr>	
	<tr>
		<td colspan="2" height="10"></td>
	</tr>
	<tr>
		<td class="subtituloCentro" width="25%">Outros cursos/ atividades</td>
		<td class="subtituloEsquerda" width="25%">SEB</td>
		<td class="subtituloEsquerda" width="25%">SECADI</td>
		<td class="subtituloEsquerda" width="25%">TOTAL</td>
	</tr>		
	<tr>
		<td class="subtituloEsquerda" width="25%">Outros cursos/ atividades - LOA (Proposta IES)</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valoratividade']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valoratividade']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valoratividade']) + desformata_valor($secadi['valoratividade'])); ?></td>
	</tr>
	<tr>
		<td class="subtituloEsquerda" width="25%">Outros cursos/ atividades</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valoroutrocurso']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valoroutrocurso']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valoroutrocurso']) + desformata_valor($secadi['valoroutrocurso'])); ?></td>
	</tr>		
	<tr>
		<td class="subtituloEsquerda" width="25%">Total Outros cursos/ atividades</td>
		<td bgcolor="#e9e9e9">R$ <?php echo $seb['valortotal']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo $secadi['valortotal']; ?></td>
		<td bgcolor="#e9e9e9">R$ <?php echo formata_valor(desformata_valor($seb['valortotal']) + desformata_valor($secadi['valortotal'])); ?></td>
	</tr>		
</table>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="10%"><b>CURSOS PROPOSTOS PELO MEC</b></td>
	</tr>
</table>

<br>

<div id="divListarCursoMEC">
	<?php listarCursosPropostoMEC(array('siftipoplanejamento' => FASE01)); ?>
</div>

<br>

<?php $habilitacurso = verificarCursoMEC(); ?>

<?php if($habilitacurso == 0){ ?>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>
		<td class="subtituloCentro" width="10%"><b>CURSOS PROPOSTOS PELA IES</b></td>
	</tr>
</table>

<?php if(!in_array(PFL_EQUIPE_MEC,$perfil) || in_array(PFL_ADMINISTRADOR,$perfil) || in_array(PFL_SUPER_USUARIO,$perfil) || ( in_array(PFL_COORDENADOR_INST,$perfil) && $esdid <> WF_PLAN_ANALISE_MEC)){ ?>
<?php if(!$planejamentofechado){ ?>
<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="80%">
	<tr>
		<td class="subtituloDireita" width="10%"><b>Incluir Curso:</b></td>
		<?php if($saldoseb <= 0 && $saldosecadi <= 0){ ?>
		<td><font color="red"><b>Não há saldo suficiente de recursos LOA.</b></font></td>	
		<?php } else { ?>

		<td>
			<input id="tcurso" type="radio" name="tcurso" value="S" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Curso do Catálogo MEC
			<input id="tcurso" type="radio" name="tcurso" value="N" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Curso fora do Catálogo MEC
			<input id="tcurso" type="radio" name="tcurso" value="A" class="normal" style="margin-top: -1px;" onclick="selecionarCurso(this.value);"/> Outras Atividades
		</td>	
		<?php } ?>		
	</tr>
</table>
<?php } ?>
<?php } ?>

<div id="divListarCursosPropostoIES">
	<?php listarCursosPropostoIES(array(),FASE01); ?>
</div>

<br>

<table align="center" bgcolor="#F5F5F5" class="tabela" cellspacing="1" cellpadding="3" width="95%">
	<tr>	
		<td class="subtituloCentro" width="10%"><b>OUTRAS ATIVIDADES</b></td>
	</tr>
</table>

<div id="divListarOutrasAtividades">
	<?php listarAtividadesPropostaIES(FASE01); ?>
</div>

<?php } ?>