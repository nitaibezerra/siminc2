<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topo.inc';

// conluindo o cadastro
include_once APPSAP . 'modulos/principal/baixadebens/concluir.inc';

// se requisicao for para salvar
if($_POST['requisicao'] == 'salvar'){

	if($oBensBaixa->existe($_POST)){
	
		// volta para a pagina de cadastro de bem
		extract($_POST);
		direcionar("?modulo=principal/baixadebens/editar&acao=A&bebid=$bebid", 'Processo de Baixa já existe.');
		
	}else{
	
		// Salva a baixa
		$bebid = $oBensBaixa->atualizar($_POST);
		$dadosBensBaixa = $oBensBaixa->pegarRegistro($bebid);
		extract($dadosBensBaixa);
		
		if($bebid){
			
			// atualiza o estado motivo do rgp
			if(!empty($_POST['bbrid'])){
				
				$oRgp->atualizarEstadoMotivo($_POST['rgpid'], $_POST['ecoid'], $_POST['mecid']);
				direcionar("?modulo=principal/baixadebens/editar&acao=A&bebid=$bebid");
			
			}else{
				
				$_POST['bebid'] = $bebid;
				// Verifica se a situação do bem é normal
				
				if($oRgp->pegarSituacao($_POST['rgpid']) == 2){				
	
					// salvando rgp
					$bbrid = $oBensBaixaRgp->salvar($_POST); 
					
					if($bbrid){
					
						// altera a situação do bem 
						$oRgp->atualizarSituacao($_POST['rgpid'], 6);
						// atualiza o estado motivo 
						$oRgp->atualizarEstadoMotivo($_POST['rgpid'], $_POST['ecoid'], $_POST['mecid']);
						// redireciona para a tela de edicao
						direcionar("?modulo=principal/baixadebens/editar&acao=A&bebid=$bebid");
						
					}
					
				}else{
					
					// mensagem de erro para a selecao de bens fora da situacao normal
					direcionar("sap.php?modulo=principal/baixadebens/editar&acao=A&bebid=$bebid", 'Apenas bens em situação Normal podem ser baixados.');				
					
				}
				
			}
					
		}else{
				
			extract($_POST);
			
		}
		
	}
	
// se for edicao de baixa de bem
}else if(!empty($_GET['bebid'])){

	$dados = $oBensBaixa->pegarRegistro($_GET['bebid']);
	extract($dados);
	$bebdata = formata_data($bebdata);
	$bebdataprocesso = formata_data($bebdataprocesso);

// se for edicao de rgp
}else if(!empty($_GET['bbrid'])){

	$dadosBaixa = $oBensBaixaRgp->pegarRegistro($_GET['bbrid']);
	
	$dadosRgp = $oRgp->carregaDadosRGP($dadosBaixa['rgpnum']);
	extract($dadosRgp);
	extract($dadosBaixa);
	
	$bebdata = formata_data($bebdata);
	$bebdataprocesso = formata_data($bebdataprocesso);
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_ADICIONA_BENS_BAIXA_ABERTO );
$db->cria_aba( $abacod_tela, $url, '',$arMnuid );

// monta o título da tela
monta_titulo('Edição de Baixa de Bens','Dados do Processo de Baixa');

// view
include_once APPSAP . 'modulos/principal/baixadebens/views/formDefault.inc';

