<?php
// topo
include_once APPSAP . 'modulos/principal/baixadebens/partialsControl/topo.inc';
require_once APPRAIZ. "www/includes/webservice/pj.php";

//caso tenha postado o formulário
if ($_POST['requisicao'] == 'salvar'){
	
	if($oBensBaixa->existe($_POST)){
	
		// volta para a pagina de cadastro de bem
		extract($_POST);
		direcionar('?modulo=principal/baixadebens/adicionar&acao=A', 'Processo de Baixa já existe.');
		
	}else{
	
		// Salva a baixa
		$bebid = $oBensBaixa->salvar($_POST);
		
		if($bebid){

			$_POST['bebid'] = $bebid;
			// Verifica se a situação do bem é normal 
			if($oRgp->pegarSituacao($_POST['rgpid']) == SBM_NORMAL){				

				// salvando rgp
				$bbrid = $oBensBaixaRgp->salvar($_POST); 
				
				if($bbrid){
				
					// altera a situação do bem 
					$oRgp->atualizarSituacao($_POST['rgpid'], SBM_BAIXA);
					// atualiza o estado motivo 
					$oRgp->atualizarEstadoMotivo($_POST['rgpid'], $_POST['ecoid'], $_POST['mecid']);
					// redireciona para a tela de edicao
					direcionar("?modulo=principal/baixadebens/editarBaixaAberto&acao=A&bebid=$bebid");
					
				}
				
			}else{
				
				// mensagem de erro para a selecao de bens fora da situacao normal
				direcionar("sap.php?modulo=principal/baixadebens/editarBaixaAberto&acao=A&bebid=$bebid", 'Apenas bens em situação Normal podem ser baixados.');				
				
			}
			
		}else{
		
			extract($_POST);
			
		}
		
	}
	
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array( ABA_EDITA_BENS_BAIXA,ABA_ADICIONA_BENS_BAIXA_ABERTO );
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo('Baixa de Bens','Dados do Processo de Baixa');

// view
include_once APPSAP . 'modulos/principal/baixadebens/views/formDefault.inc';