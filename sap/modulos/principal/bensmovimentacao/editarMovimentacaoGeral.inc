<?php
// topo
include_once APPSAP . 'modulos/principal/bensmovimentacao/partialsControl/topo.inc';

// buscar os rgps
if($_POST['requisicao'] == 'salvar'){

	extract($_POST);

	// se for insercao de movimentacao
	if(empty($mvbid)){

		$salvaMovimentacao = $oMovimentacao->salvarMovimentacao($_POST);
		$_POST['mvbid'] = $mvbid = $salvaMovimentacao[1];

	}else{

		$atualizaMovimentacao = $oMovimentacao->atualizarMovimentacao($_POST);

	}

	$situacaoRgp = $oRgp->pegarSituacao($rgpid);

	//se o bem ja existe no processo de movimentacao
	if($oBensMovimentacaoRgp->existe($mvbid, $rgpid)){

		// se tipo de movimentacao é devolução
		if($tmvid == TMV_DEVOLUCAO){

			// se rgp esta em uso
			if($situacaoRgp == SBM_USO){

				// adiciona o rgp na movimentacao
				$oBensMovimentacaoRgp->salvar(array(array('rgpid' => $rgpid)), $mvbid, $tmvid);

				// Limpando os Dados dos Bens da movimentação
				unset($rgpid,$rgpnum,$matdsc,$sbmdsc,$ecoid,$mecid,$rgpnumserie,$rgp_nu_matricula_siape,$rgp_no_servidor,$rgp_uorno,$rgp_enduf,$rgp_endcid,$rgp_endcep,$rgp_endlog,$rgp_enadescricao,$rgp_easdescricao);

			}else{

		    	alerta('O bem de RGP ' . $_POST['rgpnum'] . ' não pode ser incluído no processo de devolução pois ele não está em uso!');

			}

		}else{

			// se rgp esta em uso ou normal
			if($situacaoRgp == SBM_USO || $situacaoRgp == SBM_NORMAL){

				// adiciona o rgp na movimentacao
				$oBensMovimentacaoRgp->salvar(array(array('rgpid' => $rgpid)), $mvbid);

				// Limpando os Dados dos Bens da movimentação
				unset($rgpid,$rgpnum,$matdsc,$sbmdsc,$ecoid,$mecid,$rgpnumserie,$rgp_nu_matricula_siape,$rgp_no_servidor,$rgp_uorno,$rgp_enduf,$rgp_endcid,$rgp_endcep,$rgp_endlog,$rgp_enadescricao,$rgp_easdescricao);

			}else{

		    	alerta('O bem de RGP ' . $_POST['rgpnum'] . ' não pode ser incluído processo de movimentação pois ele está ' . $_POST['sbmdsc'] . '!');

			}

		}

	}

	// atualizando estado e motivo do rgp
	$oRgp->atualizarEstadoMotivo($_POST['rgpid'], $_POST['ecoid'], $_POST['mecid']);

// conclusão
}else if($_POST['requisicao'] == 'concluir'){

	extract($_POST);

	if($mvbid && $oBensMovimentacaoRgp->verificaExistenciaBens($mvbid) > 0){

		if($tmvid == TMV_DEVOLUCAO){

			// verificando se todos os bens da movimentacao estao com status "em devolucao"
			if($oBensMovimentacaoRgp->verificaSituacaoParaDevolucao($mvbid)){

				//pega os dados da movimentação
				$dadosMovimentacao = $oMovimentacao->pegarMovimentacao($mvbid);
				
				$_POST['ternum'] = $oTermo->gerarTermo('MV',$dadosMovimentacao['ternum']);
				$_POST['ternumanterior'] = $dadosMovimentacao['ternum'];
				
				$resultado = $oMovimentacao->concluirMovimentacao($_POST);

			}else{

				alerta('O processo de devolução não foi concluído pois existem bens já devolvidos.');

			}

		}else{

			// verificando se todos os bens da movimentacao estao com status "em movimentacao"
			if($oBensMovimentacaoRgp->verificaSituacaoDiferenteDevolucao($mvbid)){

				//pega os dados da movimentação
				$dadosMovimentacao = $oMovimentacao->pegarMovimentacao($mvbid);
				
				$_POST['ternum'] = $oTermo->gerarTermo('MV',$dadosMovimentacao['ternum']);
				$_POST['ternumanterior'] = $dadosMovimentacao['ternum'];
				
				$resultado = $oMovimentacao->concluirMovimentacao($_POST);

			}else{

				alerta('O processo de movimentação não foi concluído pois existem bens em situação diferente de Normal e Em Uso.');

			}

		}

	}else{

		alerta('A movimentação não pôde ser concluída pois não existem bens relacionados.');

	}

	if($resultado[0]){

		direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A&mvbtipo=G&mvbid='.$resultado[1], 'Operação realizada com sucesso!');

	}

// cancelar
}else if($_POST['requisicao'] == 'cancelar'){

    direcionar('?modulo=principal/bensmovimentacao/pesquisar&acao=A');


// edição
}else if(!empty($_GET['mvbid'])){

	extract($_GET);

	// recupera os dados da movimentação
	$dadosMov = $oMovimentacao->pegarMovimentacao($mvbid);
	extract($dadosMov);

	if($rgpid){
		$rgp = $oRgp->pegarRegistro($rgpid);
		// recupera rgp
		$dadosRgp = $oRgp->carregaDadosRGP($rgp['rgpnum']);

		foreach($dadosRgp as $key=>$value){

			$dadosRgp['rgp_' . $key] = $value;

		}

		extract($dadosRgp);
	}
}

$pagina = 'editarMovimentacaoGeral';

$tituloTela = (empty($mvbid) ? 'Movimentação de Bens Geral' : 'Edição de Movimentação de Bens Geral');
$subTituloTela = 'Dados do Processo de Movimentação';

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_EDITAR_MOVIMENTACAO_LOCALIDADE,ABA_EDITAR_MOVIMENTACAO_SIAPE);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

// monta o título da tela
monta_titulo($tituloTela, $subTituloTela);

// view
include_once APPSAP . 'modulos/principal/bensmovimentacao/views/formMovimentacaoGeral.inc';