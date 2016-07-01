<?php
header('content-type: text/html; charset=iso-8859-1;');
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ ."includes/workflow.php";

print "<br/>";

/*
 * SS-982 - REQ004 - GC - Consultar Solicitação
* */
$pfls = arrayPerfil();

$listaOrdemServicoExecucao	= false;
if( in_array(PERFIL_ESPECIALISTA_SQUADRA, $pfls) 
		|| in_array(PERFIL_PREPOSTO,  $pfls)
		|| in_array(PERFIL_SUPER_USUARIO,  $pfls)
		|| in_array(PERFIL_ADMINISTRADOR, $pfls)  )
{
	$listaOrdemServicoExecucao = true;
}

$usuarioRepositorio = new UsuarioRepositorio();
$usuario = $usuarioRepositorio->recuperePorId($_SESSION['usucpf']);

// Monta as Abas da Página
$menu[0] = array("descricao" => "Auditoria GC", "link"=> "fabrica.php?modulo=principal/gerencia-configuracao/listar&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=principal/gerencia-configuracao/listar&acao=A");

// Monta o Titulo da página abaixo da Aba
monta_titulo("Lista de artefatos", "<b>Dados da Solicitação de serviço</b>");

// Instanciando FiscalServico
$fiscalServico = new FiscalServico();

// Instanciando AnaliseSolicitacaoRepositorio
$analiseSolicitacaoRespositorio = new AnaliseSolicitacaoRepositorio();

// Recuperando lista de Fiscais
$fiscalRepositorio = new FiscalRepositorio();
$fiscais = $fiscalRepositorio->recupereTodos();

// Recuperando lista de Itens de Auditoria
$itemAuditoriaRespositorio = new ItemAuditoriaRepositorio();
$itensAuditoria = $itemAuditoriaRespositorio->recupereTodosSemPaginacao();

// Recuperando lista de Situacoes de Auditoria
$situacaoAuditoriaRepositorio = new SituacaoAuditoriaRepositorio();
$situacoes = $situacaoAuditoriaRepositorio->recupereTodos();

// Recuperando solicitacao
$solicitacaoRepositorio = new SolicitacaoRepositorio();
$solicitacao = $solicitacaoRepositorio->recuperePorId($_GET['solicitacao']);

// Recupera Auditoria, caso nao exista
$auditoriaRepositorio = new AuditoriaRepositorio();
$auditoria = $auditoriaRepositorio->recuperePeloIdSolicitacao( $_GET['solicitacao'] );
$servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();

$tipoDocumentoRepositorio = new TipoDocumentoRepositorio();
$tipoDocumento = $tipoDocumentoRepositorio->recuperePelaDescricao(TipoDocumento::ARTEFATOS);

//Instanciando EstadoDocumentoRepositorio
$estadoDocumentoRepositorio = new EstadoDocumentoRepositorio();

$estadoDocumentoOrigem  = 0;
$estadoDocumentoDestino = 0;

if (verificaSeTodosArtefatosForamAceitos($solicitacao->getId())){
	$estadoDocumentoOrigem = $estadoDocumentoRepositorio->recuperePorTipoDocumentoEDescricao($tipoDocumento->getId(), EstadoDocumento::ARTEFATO_PENDENTE);
	$estadoDocumentoDestino =  $estadoDocumentoRepositorio->recuperePorTipoDocumentoEDescricao($tipoDocumento->getId(), EstadoDocumento::ARTEFATO_FINALIZADA);
} else {
	$estadoDocumentoOrigem = $estadoDocumentoRepositorio->recuperePorTipoDocumentoEDescricao($tipoDocumento->getId(), EstadoDocumento::ARTEFATO_FINALIZADA);
	$estadoDocumentoDestino = $estadoDocumentoRepositorio->recuperePorTipoDocumentoEDescricao($tipoDocumento->getId(), EstadoDocumento::ARTEFATO_PENDENTE);
}

$acaoEstadoDocumentoRepositorio = new AcaoEstadoDocumentoRepositorio();
$acaoEstadoDocumento = 
	$acaoEstadoDocumentoRepositorio->recuperePorEstadoDocumentoOrigemEstadoDocumentoDestino(
		$estadoDocumentoOrigem->getId(), $estadoDocumentoDestino->getId());

if( !$listaOrdemServicoExecucao  ){

	wf_alterarEstado( $auditoria->getIdDocumento(), $acaoEstadoDocumento->getId(), 'Tramitação', array('$idSolicitacao'=>$solicitacao->getId()));
	$estadoDocumento = $estadoDocumentoRepositorio->recuperePorIdDocumento($auditoria->getIdDocumento());
}

//Gera  uma lista de id das OS como ARRAY
$os = $solicitacao->getOrdemServicoItemUm();
$listaOrdensServico = $os->recupereOrdensServicoItemUmPeloIdSolicitacao($solicitacao->getId());
foreach ($listaOrdensServico as $ordemServico){
	$listaOS[] = $ordemServico->getId();			
}

// $campos = array(
// 	'Ação'=>null,
// 	'Disciplina'=>'fsddsc',
// 	'Artefato'=>'prddsc',
// 	'Repositório'=>'sfprepositorio'
// );
	
// if (($fiscalServico->isFiscal($_SESSION['usucpf']) || $usuario->isSuperUsuario()) 
// 		&& $solicitacao->isPassivelAuditoria()) {
// 	$campos = array(
// 		'Ação'=>null,
// 		'Disciplina'=>'fsddsc',
// 		'Artefato'=>'prddsc',
// 		'Repositório'=>'sfprepositorio',
// 		'Auditar'=>null
// 	);
// }

$campos = array('Ação','Disciplina','Artefato','Repositório');

if (($fiscalServico->isFiscal($_SESSION['usucpf']) || $usuario->isSuperUsuario())
		&& $solicitacao->isPassivelAuditoria()) {
	$campos = array('Ação','Disciplina','Artefato','Repositório','Auditar');
}

$detalhesAuditoriaRepositorio 	= new DetalhesAuditoriaRepositorio();
$servicoFaseProdutoRepositorio 	= new ServicoFaseProdutoRepositorio();
$listaServicoFaseProduto 		= $servicoFaseProdutoRepositorio->recupereTodosCotratadosDaSolicitacao($_GET['solicitacao']);
$produtoContratadoServico 		= new ProdutoContratadoServico();
$colecaoProdutosContratados 	= $produtoContratadoServico->criaColecaoProdutosContratados( $listaServicoFaseProduto );

if ($colecaoProdutosContratados!=null) {
	
	$resultSet = array();

	foreach($colecaoProdutosContratados as $produtoContratado){
		
		$linha = array();
		
		$listaServicoFaseProduto = $produtoContratado->getListaServicoFaseProduto();
		$servicoFaseProduto = $listaServicoFaseProduto[0];

		$detalhesAuditoria = $detalhesAuditoriaRepositorio->recuperePorIdServicoFaseProduto($servicoFaseProduto->getId());
		
		$camposHtml = '<p style="text-align: center; width: 60px;">';
		
		if ($servicoFaseProduto->possuiRepositorio()) {
			//$camposHtml .=	'<a href="#" " style="border: none;" target="_blank">';
			$camposHtml .=		'<img src="/imagens/consultar.gif" alt="Visualizar" title="Visualizar" style="border: none;" onclick="verificaArquivoRepositorio(\''. $servicoFaseProduto->getRepositorioParaDownload() .'\')" />';
			//$camposHtml .=	'</a>';
		}
        
        $camposHtml .=		'&nbsp;&nbsp;';
        
        if( in_array(PERFIL_SUPER_USUARIO, $pfls) || !in_array(PERFIL_GERENTE_PROJETO, $pfls) || in_array(PERFIL_ADMINISTRADOR, $pfls) ){
         
    		if (!$fiscalServico->isFiscal($_SESSION['usucpf']) ) {
    			$camposHtml .=	'<img id="'.$servicoFaseProduto->getId() .'--'. $produtoContratado->getId().'" ';
    			$camposHtml .=		'title="'.$servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDescricao().'"';
    			$camposHtml .=		'class="abrirModalArtefatosCasoDeUso"';
    			$camposHtml .=		'src="/imagens/editar_nome.gif" alt="Editar" />';
    		}
        }
        
        $camposHtml .=		'&nbsp;&nbsp;';
		
		if ($detalhesAuditoria->possuiAuditoria()) {
			if ($detalhesAuditoria->auditoriaAceitavel()) {
				$camposHtml .= 	'<img src="/imagens/0_ativo.png" alt="Auditado aceito" title="Auditado aceito"/>';
			} else if ($detalhesAuditoria->auditoriaInaceitavel()){
				$camposHtml .=	'<img src="/imagens/0_inativo.png" alt="Auditado inaceitável" title="Auditado inaceitável" />';
			} else {
				$camposHtml .=	'<img src="/imagens/0_inexistente.png" alt="Não auditado" title="Não auditado"/>';
			}
		} else {
			$camposHtml .=	'<img src="/imagens/0_inexistente.png" alt="Não auditado" title="Não auditado"/>';
		}
		$camposHtml .=	'</p>';
		
		$linha[] = $camposHtml;
		$linha[] = $servicoFaseProduto->getFaseDisciplinaProduto()->getFaseDisciplina()->getDisciplina()->getDescricao();
		$linha[] = $servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDescricao();
		$linha[] = $servicoFaseProduto->getRepositorio();
		
		$botaoAuditar = '';
		if (($fiscalServico->isFiscal($_SESSION['usucpf']) || $usuario->isSuperUsuario()) 
				&& $solicitacao->isPassivelAuditoria() 
					&& $servicoFaseProduto->possuiRepositorio()) {
			
			$botaoAuditar  =	'<input class="gc-auditarArtefato"';
			$botaoAuditar .=		'title="'.$servicoFaseProduto->getFaseDisciplinaProduto()->getProduto()->getDescricao().'"';
			$botaoAuditar .=		'alt="'.$servicoFaseProduto->getId() .'--'. $produtoContratado->getId().'"';
			$botaoAuditar .=		'type="button" value="Auditar"/>';
		}
		$linha[] = $botaoAuditar;
		$resultSet[] = $linha;
	}
}

?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="./css/jquery-ui-1.8.16.custom.css" type="text/css" rel="stylesheet" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="./js/jquery.validate.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>


<form id="form-listaArtefatosGc" method="post" action="" style="text-align: center;">

	<input type="hidden" name="idSS" value="<?php echo $solicitacao->getId()?>" />
	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubtituloDireita" width="25%">Número de solicitação:</td>
				<td>
					<span id="idSS"><?php echo $solicitacao->getId();?></span>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" width="25%">Sistema:</td>
				<td>
					<?php echo $solicitacao->getSistema()->getDescricao();?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" width="25%">Número de Ordem de Serviço:</td>
				<td>
					<?php 
						echo implode(", ", $listaOS);
					?>
				</td>
			</tr>
		</table>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 95%;">
			<tr>
				<td colspan="2" class="SubTituloCentro" align="center">Dados da Auditoria GC</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" width="25%">Fiscal:</td>
				<td>
					<select name="gc-fiscais" class="campoEstilo">
						<option value="">Selecione</option>
						<?php
						
						foreach ($fiscais as $fiscal){
							if( !$listaOrdemServicoExecucao  ){
								if ($solicitacao->possuiAuditoria()) {
									if ($solicitacao->getAuditoria()->getFiscal()->getId() == $fiscal->getId()){
										$fiscalSelected = 'selected="selected"';
									} else {
										$fiscalSelected = '';
									}
								}
							}?>
							<option <?php echo $fiscalSelected; ?> 
								value="<?php echo $fiscal->getId();?>"><?php echo $fiscal->getNome();?></option>
						<?php }?>
					</select>
					<?php if( !$listaOrdemServicoExecucao  ){ ?>
					<span id="<?php echo $solicitacao->getAuditoria()->getId();?>" 
						class="linkVerHistorico">Ver Histórico</span>
					<?php } ?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" width="25%">Responsável fábrica:</td>
				<td>
					<?php 
					
					if ($solicitacao->possuiAuditoria()) {
						$auditoria = $solicitacao->getAuditoria();
						$nomeResponsavel = $solicitacao->getAuditoria()->getNomeResponsavelFabrica();
					}?>
					<? echo campo_texto('audrespfabrica', 'N',  'S', "", 50, 200, '',
	                    '', '', '', 0, 'id="audrespfabrica"', '', $nomeResponsavel, '', ''); ?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" width="25%">Situação:</td>
				<td>
					<?php
					if( !$listaOrdemServicoExecucao ){
						print $estadoDocumento->getDescricao();
					}
					?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" width="25%"></td>
                <td>
					<input id="gc-analiseSolicitacao" type="hidden" name="gc-analiseSolicitacao"
						value="<?php echo $solicitacao->getAnaliseSolicitacao()->getId(); ?>" />
			        <?php if( in_array(PERFIL_SUPER_USUARIO, $pfls) || !in_array(PERFIL_GERENTE_PROJETO, $pfls) || in_array(PERFIL_ADMINISTRADOR, $pfls) ){ ?>
					<input id="gc-salvarAuditoriaArtefatos" type="submit" value="Salvar" />
					<?php } ?>
					<input type="button" value="Voltar" onclick="parent.location='?modulo=principal/gerencia-configuracao/listar&acao=A&redirecionamento=1'"/>
				</td>
			</tr>
		</table>
		
</form>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" class="SubTituloCentro" align="center">Lista de Artefatos</td>
	</tr>
</table>

<?php
		$tamanho       = array("20%","15%","15%","45%","5%");
		$alinhamento   = array("center","left","left","left","center");
		$db->monta_lista_array($resultSet,$campos,20,5,'N','center',$par2,"",$tamanho,$alinhamento);
?>

<div id="modalArtefatosCasoDeUso">
	<form id="formModalArtefatosCasoDeUso" action="">
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubtituloDireita">Repositório:</td>
				<td>
					<?php $valor = "";?>
					<?php echo campo_texto('repositorioCasoDeUso', 'S', 'S', "", 47, 300, '',
	                    '', '', '', 0, 'id="repositorioCasoDeUso"', '', $valor, '', ''); ?>
	            </td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Utilizando padrão de nomes:</td>
				<td>
					<select name="padraoNomesCasoDeUso" class="campoEstilo">
						<option value="">Selecione</option>
						<option id="padraoNomeSimCasoDeUso" value="S">Sim</option>
						<option id="padraoNomeNaoCasoDeUso" value="N">Não</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Utilizando padrão de diretórios:</td>
				<td>
					<select name="padraoDiretorioCasoDeUso" class="campoEstilo">
						<option value="">Selecione</option>
						<option id="padraoDiretorioSimCasoDeUso" value="S">Sim</option>
						<option id="padraoDiretorioNaoCasoDeUso" value="N">Não</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Encontrado:</td>
				<td>
					<select name="encontradoCasoDeUso" class="campoEstilo">
						<option value="">Selecione</option>
						<option id="encontradoSimCasoDeUso" value="S">Sim</option>
						<option id="encontradoNaoCasoDeUso" value="N">Não</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Atualizado:</td>
				<td>
					<select name="atualizadoCasoDeUso" class="campoEstilo">
						<option value="">Selecione</option>
						<option id="atualizadoSimCasoDeUso" value="S">Sim</option>
						<option id="atualizadoNaoCasoDeUso" value="N">Não</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Necessário:</td>
				<td>
					<select name="necessarioCasoDeUso" class="campoEstilo">
						<option value="">Selecione</option>
						<option id="necessarioSimCasoDeUso" value="S">Sim</option>
						<option id="necessarioNaoCasoDeUso" value="N">Não</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
                <td class="SubtituloEsquerda" colspan="2" style="text-align: center">
					<input id="idServicoFaseProdutoCasoDeUso" name="idServicoFaseProdutoCasoDeUso" type="hidden" value="" />
					<input id="salvarModalArtefatosCasoDeUso" type="button" value="Salvar" />
					<input id="cancelarModalArtefatosCasoDeUso" type="button" value="Cancelar" />
				</td>
			</tr>
		</table>
	</form>
</div>

<div id="modalArtefatosVisaoMEC">
	<form id="formModalArtefatosVisaoMEC" action="">
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center" bgcolor="#f5f5f5">
			<tr>
				<td colspan="2" class="SubTituloCentro" align="center">Visão MEC</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Repositório:</td>
				<td><a id="repositorioVisaoMEC" href="" target="_blank" style="color: blue;"></a></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Utilizando o padrão de nomes:</td>
				<td><span id="padraoNomeVisaoMEC"></span></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Utilizando padrão de diretórios:</td>
				<td><span id="padraoDiretorioVisaoMEC"></span></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Encontrado:</td>
				<td><span id="encontradoVisaoMEC"></span></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Atualizado:</td>
				<td><span id="atualizadoVisaoMEC"></span></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Necessário:</td>
				<td><span id="necessarioVisaoMEC"></span></td>
			</tr>
		</table>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="3" class="SubTituloCentro" align="center">Auditoria MEC</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Auditoria:</td>
				<td colspan="2">
					<select id="resultadoAuditoriaVisaoMEC" name="resultadoAuditoriaVisaoMEC" class="campoEstilo"> 
						<option value="0">Selecione</option>
						<option id="resultadoAceitavelVisaoMEC" value="1">Aceitável</option>
						<option id="resultadoAceitavelRestricaoVisaoMEC" value="2">Aceitável com restrição</option>
						<option id="resultadoInaceitavelVisaoMEC" value="3">Inaceitável</option>
					</select>
					<label id="error_resultado_auditoria" class="error" style="display: none;">Campo Obrigatório</label>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Itens de auditoria:</td>
				<td>
					<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
						<tr>
					
						<?php for ($x=1; $x <= count($itensAuditoria) ; $x++) {?>
							<?php $itemAuditoria = $itensAuditoria[$x - 1]; ?>
							<td>
								<input type="checkbox" name="itemAuditoria[]" value="<?= $itemAuditoria->getId();?>" />
								<label><?=$itemAuditoria->getDescricao();?></label>
							</td>
							
							<?php if($x % 2 == 0) {?>
								</tr>
							<?php } ?>
						<?php } ?>
						
					</table>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Motivo:</td>
				<td colspan="2">
					<?php
						$numeroDeLinhas = 60;
						$numeroDeColunas = 4;
						$maximoCaracteres = 2000;
						$textoPadrao = "";
					?>
					<?php echo campo_textarea('motivoAuditoriaVisaoMEC', 'N', $habil, null, $numeroDeLinhas, $numeroDeColunas , $maximoCaracteres, null, null, null, null, null,$textoPadrao); ?>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Observação:</td>
				<td colspan="2">
					<?php
						$textoPadrao = "";
					?>
					<?php echo campo_textarea('observacaoAuditoriaVisaoMEC', 'N', $habil, null, $numeroDeLinhas, $numeroDeColunas , $maximoCaracteres, null, null, null, null, null,$textoPadrao); ?>
				</td>
			</tr>
			<tr>
                <td class="SubtituloEsquerda" colspan="3" style="text-align: center;" >
					<input id="idServicoFaseProdutoVisaoMEC" name="idServicoFaseProdutoVisaoMEC" type="hidden" value="" />
					<input id="idAuditoriaVisaoMEC" name="idAuditoriaVisaoMEC" type="hidden" value="" />
					<input id="idDetalhesAuditoriaVisaoMEC" name="idDetalhesAuditoriaVisaoMEC" type="hidden" value="" />
					<input id="salvarModalArtefatosVisaoMEC" type="button" value="Salvar" />
					<input id="cancelarModalArtefatosVisaoMEC" type="button" value="Cancelar" />
				</td>
			</tr>
		</table>
	</form>
</div>

<div id="modalVerHistorico"></div>
<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>


<script type="text/javascript" src="./js/gerencia-configuracao.js"></script>
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


function verificaArquivoRepositorio(urlRepositorio)
{
    var params = { 'repositorioCasoDeUso' : urlRepositorio };
    $.ajax({
            beforeSend: function(){
                $("#dialogAjax").show();
            },
            type: 'post',
            url :'geral/gerencia-configuracao/validarArtefato.php',
            cache: false,
            dataType: 'json',
            data: params,
            success: function(data){
            	if (data.STATUS != "ERROR"){
                    window.open( urlRepositorio, '_blank' );
            	} else {
            		alert( data.MENSAGEM );
            	}
            },
		
            complete: function(){
                $("#dialogAjax").hide();
            }
        });
}
</script>