<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$usuarioRepositorio = new UsuarioRepositorio();
$usuario = $usuarioRepositorio->recuperePorId($_SESSION['usucpf']);

//Instanciando FiscalServico
$fiscalServico = new FiscalServico();

//Instanciando DetalhesAuditoriaReposiorio
$detalhesAuditoriaRepositorio = new DetalhesAuditoriaRepositorio();

$orderBy = $_POST['campo'] == null ? 'fsddsc' : $_POST['campo'] ;
$ordem = $_POST['ordem'] == null ? Ordenador::ORDEM_PADRAO : $_POST['ordem'];
$limit = $_POST['limit'] == null ? Paginador::LIMIT_PADRAO : $_POST['limit'];
$offset = $_POST['offset'] == null ? Paginador::OFFSET_PADRAO : $_POST['offset'];

//Recuperando solicitacao
$solicitacaoRepositorio = new SolicitacaoRepositorio();
$solicitacao = $solicitacaoRepositorio->recuperePorId($_POST['idSS']);

$servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();
$listaServicoFaseProduto = $servicoFaseProdutoRepositorio->recupereCotratadosDaSolicitacao($_POST['idSS'], $orderBy, $ordem, $limit, $offset);

$produtoContratadoServico = new ProdutoContratadoServico();
$colecaoProdutosContratados = $produtoContratadoServico->criaColecaoProdutosContratados($listaServicoFaseProduto);

$auditoriaRepositorio = new AuditoriaRepositorio();
$auditoria = $auditoriaRepositorio->recuperePeloIdSolicitacao($_POST['idSS']);

if ($colecaoProdutosContratados!=null) {
	foreach($colecaoProdutosContratados as $produtoContratado){
		
		$listaServicoFaseProduto = $produtoContratado->getListaServicoFaseProduto();
		$servicoFaseProduto = $listaServicoFaseProduto[0];
		
		if($x % 2 == 0){
			$cssClass = 'odd';
		} else {
			$cssClass = 'even';
		}

		$detalhesAuditoria = $detalhesAuditoriaRepositorio->recuperePorIdServicoFaseProduto($servicoFaseProduto->getId());
		
		
?>
		
		<tr class="<?echo $cssClass;?>">
			<td class="botoesDeAcao" width="7%">
				<?php if ($servicoFaseProduto->possuiRepositorio()) {?>
					<a href="<?php echo $servicoFaseProduto->getRepositorioParaDownload();?>" style="border: none;" target="_blank">
						<img src="/imagens/consultar.gif" alt="Visualizar" title="Visualizar" style="border: none;"/>
					</a>
				<?php } ?>
				
				<?php if (!$fiscalServico->isFiscal($_SESSION['usucpf'])) { ?>
					<img id="<?php echo $servicoFaseProduto->getId() ."--". $produtoContratado->getId();?>" 
						title="<?php echo $servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDescricao();?>"
						class="abrirModalArtefatosCasoDeUso"
						src="/imagens/editar_nome.gif" alt="Editar" />
				<?php } ?>
				
					
				<?php if ($detalhesAuditoria->possuiAuditoria()) {?>
					<?php if ($detalhesAuditoria->auditoriaAceitavel()) {?>
						<img src="/imagens/0_ativo.png" alt="Auditado aceito" title="Auditado aceito"/>
					<?php } else if ($detalhesAuditoria->auditoriaInaceitavel()){?>
						<img src="/imagens/0_inativo.png" alt="Auditado inaceitável" title="Auditado inaceitável" />
					<?php } else {?>
						<img src="/imagens/0_inexistente.png" alt="Não auditado" title="Não auditado"/>
					<?php }?>
				<?php } else {?>
					<img src="/imagens/0_inexistente.png" alt="Não auditado" title="Não auditado"/>
				<?php } ?>
				
			</td>
			<td class="left"><?php echo $servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDisciplina()->getDescricao();?></td>
			<td class="left"><?php echo $servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDescricao();?></td>
			<td class="left"><?php echo $servicoFaseProduto->getRepositorio();?></td>
			<?php if (($fiscalServico->isFiscal($_SESSION['usucpf']) || $usuario->isSuperUsuario()) 
						&& $solicitacao->isPassivelAuditoria()) { ?>
				<td class="alignCenter">
					<?php if ($servicoFaseProduto->possuiRepositorio()) {?>
						<input class="gc-auditarArtefato" 
							title="<?php echo $servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDescricao();?>"
							alt="<?php echo $servicoFaseProduto->getId() ."--". $produtoContratado->getId();?>" 
							type="button" value="Auditar"/>
					<?php } ?>
				</td>
			<?php } ?>
		</tr>
<?php } 
} else {
?><tr><td colspan="5" class="alignCenter">Não foram encontrados registros</td></tr><?php 
}
?>
<script type="text/javascript">
$(".gc-auditarArtefato").click(function(event){
    event.preventDefault();
    var id = $(this).attr('alt');
    var nomeProduto = $(this).attr('title');
    $.ajax({
        beforeSend: function(){
            $("#dialogAjax").show();
        },
        type: 'post',
        url:'geral/gerencia-configuracao/recuperarArtefatoVisaoMEC.php',
        cache: false,
        dataType: 'html',
        data: "idServicoFaseProduto=" + id,
        success: function(data){
//            console.log(data);
            detalhesAuditoria = $.parseJSON(data);

			$.each(detalhesAuditoria.itensAuditoriaAssociados, function(idx, elem){
				$('input[value="' + elem.id + '"]').attr("checked", "checked");
			});
			
            $("#repositorioVisaoMEC").attr('href', detalhesAuditoria.servicoFaseProduto.repositorioHttp);
            $("#repositorioVisaoMEC").html(detalhesAuditoria.servicoFaseProduto.repositorio);
			
            if (detalhesAuditoria.servicoFaseProduto.padraoNome == "t"){
                detalhesAuditoria.servicoFaseProduto.padraoNome = "SIM";
            } else if (detalhesAuditoria.servicoFaseProduto.padraoNome == "f"){
                detalhesAuditoria.servicoFaseProduto.padraoNome = "NÃO";
            }
            $("#padraoNomeVisaoMEC").html(detalhesAuditoria.servicoFaseProduto.padraoNome);
			
            if (detalhesAuditoria.servicoFaseProduto.padraoDiretorio == "t"){
                detalhesAuditoria.servicoFaseProduto.padraoDiretorio = "SIM";
            } else if (detalhesAuditoria.servicoFaseProduto.padraoDiretorio == "f"){
                detalhesAuditoria.servicoFaseProduto.padraoDiretorio = "NÃO";
            }
            $("#padraoDiretorioVisaoMEC").html(detalhesAuditoria.servicoFaseProduto.padraoDiretorio);
			
            if (detalhesAuditoria.servicoFaseProduto.encontrado == "t"){
                detalhesAuditoria.servicoFaseProduto.encontrado = "SIM";
            } else if (detalhesAuditoria.servicoFaseProduto.encontrado == "f"){
                detalhesAuditoria.servicoFaseProduto.encontrado = "NÃO";
            }
            $("#encontradoVisaoMEC").html(detalhesAuditoria.servicoFaseProduto.encontrado);
			
            if (detalhesAuditoria.servicoFaseProduto.atualizado == "t"){
                detalhesAuditoria.servicoFaseProduto.atualizado = "SIM";
            } else if (detalhesAuditoria.servicoFaseProduto.atualizado == "f"){
                detalhesAuditoria.servicoFaseProduto.atualizado = "NÃO";
            }
            $("#atualizadoVisaoMEC").html(detalhesAuditoria.servicoFaseProduto.atualizado);
			
            if (detalhesAuditoria.servicoFaseProduto.necessario == "t"){
                detalhesAuditoria.servicoFaseProduto.necessario = "SIM";
            } else if (detalhesAuditoria.servicoFaseProduto.necessario == "f"){
                detalhesAuditoria.servicoFaseProduto.necessario = "NÃO";
            }
            $("#necessarioVisaoMEC").html(detalhesAuditoria.servicoFaseProduto.necessario);
			
            $("#idServicoFaseProdutoVisaoMEC").val(detalhesAuditoria.servicoFaseProduto.id);
            $("#idAuditoriaVisaoMEC").val(detalhesAuditoria.auditoria.id);
			
            $("#idDetalhesAuditoriaVisaoMEC").val(detalhesAuditoria.id);
            $("#motivoAuditoriaVisaoMEC").val(detalhesAuditoria.motivo);
            $("#observacaoAuditoriaVisaoMEC").val(detalhesAuditoria.observacao);
			
            if (detalhesAuditoria.resultado == 1){
                $("#resultadoAceitavelVisaoMEC").attr('selected', 'selected');
            } else if (detalhesAuditoria.resultado == 2){
                $("#resultadoAceitavelRestricaoVisaoMEC").attr('selected', 'selected');
            } else if (detalhesAuditoria.resultado == 3){
                $("#resultadoInaceitavelVisaoMEC").attr('selected', 'selected');
            }
			
            $("#modalArtefatosVisaoMEC").dialog({
                title: "Artefato: " + nomeProduto
            });
			
            $("#modalArtefatosVisaoMEC").dialog('open');
        },
        complete: function(){
			$('#aguarde').css('visibility', 'hidden');
			$('#aguarde').hide();
        },
        error: function(){
			$('#aguarde').css('visibility', 'hidden');
			$('#aguarde').hide();
            alert("Atribua um fiscal responsável pela auditoria.");
        }
    });
	
});

$(".abrirModalArtefatosCasoDeUso").click(function(event){
    event.preventDefault();
    var id = $(this).attr('id');
    var nomeProduto = $(this).attr('title');
    $.ajax({
        beforeSend: function(){
            $("#dialogAjax").show();
        },
        type: 'post',
        url:'geral/gerencia-configuracao/recuperarArtefato.php',
        cache: false,
        dataType: 'html',
        data: "idServicoFaseProduto=" + id,
        success: function(data){
            servicoFaseProduto = $.parseJSON(data);
			
            $("#idServicoFaseProdutoCasoDeUso").val(servicoFaseProduto.id);
			
            $("#repositorioCasoDeUso").val(servicoFaseProduto.repositorio);
			
            if (servicoFaseProduto.padraoNome == "t"){
                $("#padraoNomeSimCasoDeUso").attr('selected', 'selected');
            } else if (servicoFaseProduto.padraoNome == "f") {
                $("#padraoNomeNaoCasoDeUso").attr('selected', 'selected');
            }
			
            if (servicoFaseProduto.padraoDiretorio == "t"){
                $("#padraoDiretorioSimCasoDeUso").attr('selected', 'selected');
            } else if (servicoFaseProduto.padraoDiretorio == "f") {
                $("#padraoDiretorioNaoCasoDeUso").attr('selected', 'selected');
            }
			
            if (servicoFaseProduto.encontrado == "t"){
                $("#encontradoSimCasoDeUso").attr('selected', 'selected');
            } else if (servicoFaseProduto.encontrado == "f") {
                $("#encontradoNaoCasoDeUso").attr('selected', 'selected');
            }
			
            if (servicoFaseProduto.atualizado == "t"){
                $("#atualizadoSimCasoDeUso").attr('selected', 'selected');
            } else if (servicoFaseProduto.atualizado == "f") {
                $("#atualizadoNaoCasoDeUso").attr('selected', 'selected');
            }
			
            if (servicoFaseProduto.necessario == "t"){
                $("#necessarioSimCasoDeUso").attr('selected', 'selected');
            } else if (servicoFaseProduto.necessario == "f") {
                $("#necessarioNaoCasoDeUso").attr('selected', 'selected');
            }
			
            $("#modalArtefatosCasoDeUso").dialog({
                title: "Artefato: " + nomeProduto
                });
			
            $("#modalArtefatosCasoDeUso").dialog("open");
        },
		
        complete: function(){
            $("#dialogAjax").hide();
        }
		
    });
});
</script>