<?php

class Controller_Default extends Abstract_Controller {

    public function __construct() {
        parent::__construct();
        $this->view->titulo = 'Planos Táticos Operacionais';
		$this->view->where = null;
		$this->view->dados = array();
        $this->view->perfilUsuario = new Model_PerfilUsuario();
        $this->view->perfilUsuario->validaAcesso();

        $this->view->solucao = new Model_Solucao(false);
        $this->view->tema = new Model_Tema(false);
        $this->view->etapa = new Model_Etapa(false);

        $this->view->temaSolucao = new Model_Temasolucao(false);
        $this->view->acaoSolucao = new Model_Acaosolucao(false);
        $this->view->metaSolucao = new Model_Metasolucao(false);
        $this->view->indicadorSolucao = new Model_Indicadorsolucao(false);
        $this->view->responsavelSolucaoSe = new Model_Responsavelsolucao(false);
        $this->view->responsavelSolucaoSeAut = new Model_Responsavelsolucao(false);
        $this->view->secretaria = new Model_Secretaria(false);
        $this->view->secretariaSolucao = new Model_Secretariasolucao(false);
        $this->view->aeObjetivoEstrategico = new Model_AeObjetivoEstrategico(false);
        $this->view->aeIniciativa = new Model_AeIniciativa(false);
        $this->view->aeEstrategia = new Model_AeEstrategia(false);
        $this->view->estrategiaSolucao = new Model_EstrategiaSolucao(false);
        $this->view->objetivoSolucao = new Model_ObjetivoSolucao(false);
        $this->view->iniciativaSolucao = new Model_IniciativaSolucao(false);
        $this->view->aeArtigo = new Model_AeArtigo(false);
        $this->view->artigoSolucao = new Model_ArtigoSolucao(false);
    }

    public function indexAction() {
        $_SESSION['solid'] = null;
        $_SESSION['etpid'] = null;
        $_SESSION['atvid'] = null;
        $_SESSION['acaids'] = null;
        $_SESSION['secid'] = null;
		$_SESSION['estid'] = null;
        $this->view->data = $this->view->solucao->getDadosGrid();
        $this->view->listing = $this->view->solucao->getListing();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction() {
        $params = array();
        parse_str($_POST['parans'], $params);

        $this->view->data = $this->view->solucao->getDadosGrid($params);
        $this->view->listing = $this->view->solucao->getListing();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction() {
        try {
            $arrayTemas = $_POST['temid'];
            $arrayAcoesEstrategicas = $_POST['acaid'];
            $arrayMetas = $_POST['mpneid'];
            $arrayIndicador = $_POST['indid'];
            $arrayResponsavelSe = $_POST['resid_se'];
            $arrayResponsavelSecretariaAutarquia = $_POST['resid_se_au'];
            $arrayObjetivoSolucao = $_POST['obeid'];
            $arrayIniciativaSolucao = $_POST['iniid'];
            $arraySecretaria = $_POST['secid'];
            $arrayEstrategias = $_POST['estid'];
            $arrayArtigos = $_POST['artid'];

            $posIdCorpoLei = $this->view->metaSolucao->validaCorpoLei($arrayMetas);
            if($posIdCorpoLei !== false && count($arrayMetas) > 1){
                unset($arrayMetas[$posIdCorpoLei]);
                $this->view->solucao->setAttributeValue('solcorpolei', 't');
            }

            $idSolucao = $this->view->solucao->inserirSolucao($arrayMetas);
            if (!empty($idSolucao)) {
                /** SALVA TODOS OS DADOS ADICIONAIS */
                $this->view->temaSolucao->salvarTema($arrayTemas, $idSolucao);
                $this->view->acaoSolucao->salvarAcao($arrayAcoesEstrategicas, $idSolucao);
                $this->view->secretariaSolucao->salvarSecretaria($arraySecretaria, $idSolucao);
				$this->view->indicadorSolucao->salvarIndicador($arrayIndicador, $idSolucao);
                $this->view->responsavelSolucaoSe->salvarResponsavelSe($arrayResponsavelSe, $idSolucao);
                $this->view->responsavelSolucaoSeAut->salvarResponsavelSecretariaAutarquia($arrayResponsavelSecretariaAutarquia, $idSolucao);
				$this->view->iniciativaSolucao->salvarIniciativaSolucao($arrayIniciativaSolucao, $idSolucao);
				$this->view->estrategiaSolucao->salvarEstrategia($arrayEstrategias, $idSolucao, $arrayMetas);
				$this->view->objetivoSolucao->salvarObjetivoSolucao($arrayObjetivoSolucao, $idSolucao, $arrayMetas);
				$this->view->artigoSolucao->salvarArtigo($arrayArtigos, $idSolucao, $arrayMetas);


                $validoCodigoSuporte = array_search('6', $arrayTemas) !== false && count($arrayTemas) > 1;
                if ($validoCodigoSuporte) {
                    $this->view->temaSolucao->error[] = array("name" => 'temid', "msg" => ('Selecione somente o tema "Suporte" ou remova da lista'));
                    throw new Exception('Selecione somente o tema "Suporte" ou remova ');
                } elseif (array_search('6', $arrayTemas) !== false) {
                    $this->view->metaSolucao->deleteAllByValues(array('solid' => $idSolucao));
                } else {
                    if (is_array($arrayMetas)) {
                        $this->view->metaSolucao->deleteAllByValues(array('solid' => $idSolucao));

                        if (array_search('nenhuma', $arrayMetas) === false) {
                            $this->view->metaSolucao->salvarMeta($arrayMetas, $idSolucao);
                        }
                        if (array_search('nenhuma', $arrayMetas) !== false and empty($_POST['solmetajustificativa'])) {
                            $this->view->solucao->error[] = array("name" => 'solmetajustificativa', "msg" => ('Não pode estar vazio'));
                            throw new Exception('Justificativa da Meta não pode ser vazio!');
                        }
                    } else {
                        $this->view->metaSolucao->error[] = array("name" => 'mpneid', "msg" => ('Não pode estar vazio'));
                        throw new Exception('Nenhuma Meta PNE Selecionada!');
                    }
                }

                $tituloSolucao = $this->view->solucao->getTituloSolucao();

                $_SESSION['solid'] = $idSolucao;
                $this->carregaSolucaoById($idSolucao);

                $this->executeCommit();
                $return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO), 'tituloSolucao' => $tituloSolucao, 'idSolucao' => $idSolucao);
            }
        } catch (Exception $e) {
            $this->executeRollback();
            $error = $this->getErrors();
            $return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $error);
        }
        echo simec_json_encode($return);
    }

    public function cadastrarAction() {
        $solid = (int) $this->getPost('id');
        $solid = (empty($solid) ? $_SESSION['solid'] : $solid);
        if (!empty($solid)) {
            $this->carregaSolucaoById($solid);
        }

        $this->carregarSelecionados($solid, 'temid', 'tema', 'temaSolucao');
        $this->carregarSelecionados($solid, 'obeid', 'aeObjetivoEstrategico', 'objetivoSolucao');
        $this->carregarSelecionados($solid, 'iniid', 'aeIniciativa', 'iniciativaSolucao');
        $this->carregarSelecionados($solid, 'secid', 'secretaria', 'secretariaSolucao');
        $this->carregarSelecionados($solid, 'acaid', 'acaoSolucao');
        $this->carregarSelecionados($solid, 'mpneid', 'metaSolucao');
        $this->carregarSelecionados($solid, 'indid', 'indicadorSolucao');
        $this->carregarSelecionados($solid, 'usucpf', 'responsavelSolucaoSe');
        $this->carregarSelecionados($solid, 'usucpf', 'responsavelSolucaoSeAut');
        $this->carregarSelecionados($solid, 'estid', 'aeEstrategia', 'estrategiaSolucao');
        $this->carregarSelecionados($solid, 'artid', 'aeArtigo', 'artigoSolucao');

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function carregarIndicadorAction() {
        $acaid = (int) $_POST['acaid'][0];
        $marcador = $this->getPost('marcador');

        if ($marcador == 'adicionar') {
            $_SESSION['acaids'][] = $acaid;
        } elseif ($marcador == 'remover') {
            $indice = array_search($acaid, $_SESSION['acaids']);
            unset($_SESSION['acaids'][$indice]);
        }

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function excluirAction() {
        $solid = (int) $this->getPost('id');

        if ($this->view->etapa->possuiEtapa($solid)) {
            $return = array('status' => false, 'msg' => (self::REGISTRO_POSSUI_VINCULO));
        } else {
            try {
                $this->view->solucao->inativar($solid);
                $this->view->solucao->commit();
                $return = array('status' => false, 'msg' => (self::DADOS_EXCLUIDOS_COM_SUCESSO), 'result' => '', 'type' => 'success');
            } catch (Exception $e) {
                $this->view->solucao->rollback();
                $return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $e->getMessage());
            }
        }
        echo simec_json_encode($return);
    }

    function carregarSelecionados($solid, $nomeCampoId, $model, $model_busca = null) {
        if (is_null($model_busca)) {
            $model_busca = $model;
        }
        $ids = array();
        if (!empty($solid)) {
            if ($model == 'responsavelSolucaoSe') {
                $arrayIds = $this->view->$model_busca->getAllByValues(array('solid' => $solid, 'restipo' => 'S'));
            } elseif ($model == 'responsavelSolucaoSeAut') {
                $arrayIds = $this->view->$model_busca->getAllByValues(array('solid' => $solid, 'restipo' => 'A'));
            } else {
                $arrayIds = $this->view->$model_busca->getAllByValues(array('solid' => $solid));
            }
            if (is_array($arrayIds)) {
                foreach ($arrayIds as $id) {
                    $ids[] = $id[$nomeCampoId];
                }
            }
        }
		if( $this->view->solucao->getAttributeValue('solcorpolei') == 't' and $nomeCampoId == 'mpneid' ){
			$_SESSION['mpneid'][] = Model_Metasolucao::CORPO_LEI_ID;
			$ids[] =  Model_Metasolucao::CORPO_LEI_ID;
		}
        if ($nomeCampoId == 'acaid') {
            $_SESSION['acaids'] = $ids;
        }
        $this->view->$model->setAttributeValue($nomeCampoId, $ids);
        $this->view->dados = array_merge( array($nomeCampoId=>$ids), $this->view->dados);
    }

    function getErrors() {
        $errorSolucao = (is_array($this->view->solucao->error) ? $this->view->solucao->error : array());
        $errorTemaSolucao = (is_array($this->view->temaSolucao->error) ? $this->view->temaSolucao->error : array());
        $errorSecretaria = (is_array($this->view->secretariaSolucao->error) ? $this->view->secretariaSolucao->error : array());
        $errorAeObjetivoEstrategico = (is_array($this->view->aeObjetivoEstrategico->error) ? $this->view->aeObjetivoEstrategico->error : array());
        $errorObjetivoSolucao = (is_array($this->view->objetivoSolucao->error) ? $this->view->objetivoSolucao->error : array());
		$errorAeIniciativa = (is_array($this->view->aeIniciativa->error) ? $this->view->aeIniciativa->error : array());
        $errorIniciativaSolucao = (is_array($this->view->iniciativaSolucao->error) ? $this->view->iniciativaSolucao->error : array());
        $errorAcaoSolucao = (is_array($this->view->acaoSolucao->error) ? $this->view->acaoSolucao->error : array());
        $errorMetaSolucao = (is_array($this->view->metaSolucao->error) ? $this->view->metaSolucao->error : array());
        $errorIndicadorSolucao = (is_array($this->view->indicadorSolucao->error) ? $this->view->indicadorSolucao->error : array());
        $errorResponsavelSolucaoSe = (is_array($this->view->responsavelSolucaoSe->error) ? $this->view->responsavelSolucaoSe->error : array());
        $errorResponsavelSolucaoSeAut = (is_array($this->view->responsavelSolucaoSeAut->error) ? $this->view->responsavelSolucaoSeAut->error : array());
        $errorEstrategiaSolucao = (is_array($this->view->estrategiaSolucao->error) ? $this->view->estrategiaSolucao->error : array());
        $errorArtigoSolucao = (is_array($this->view->artigoSolucao->error) ? $this->view->artigoSolucao->error : array());

        return array_merge($errorSolucao, $errorTemaSolucao, $errorAcaoSolucao, $errorMetaSolucao, $errorIndicadorSolucao, $errorResponsavelSolucaoSe,
			$errorResponsavelSolucaoSeAut, $errorAeObjetivoEstrategico,$errorObjetivoSolucao, $errorAeIniciativa,$errorIniciativaSolucao,$errorSecretaria, $errorEstrategiaSolucao, $errorArtigoSolucao);
    }

    function executeRollback() {
        $this->view->solucao->rollback();
        $this->view->temaSolucao->rollback();
        $this->view->objetivoSolucao->rollback();
        $this->view->iniciativaSolucao->rollback();
        $this->view->acaoSolucao->rollback();
        $this->view->metaSolucao->rollback();
        $this->view->indicadorSolucao->rollback();
        $this->view->responsavelSolucaoSe->rollback();
        $this->view->responsavelSolucaoSeAut->rollback();
        $this->view->aeObjetivoEstrategico->rollback();
        $this->view->secretariaSolucao->rollback();
        $this->view->estrategiaSolucao->rollback();
        $this->view->artigoSolucao->rollback();
    }

    function executeCommit() {
        $this->view->solucao->commit();
        $this->view->temaSolucao->commit();
        $this->view->objetivoSolucao->commit();
		$this->view->iniciativaSolucao->commit();
        $this->view->acaoSolucao->commit();
        $this->view->metaSolucao->commit();
        $this->view->indicadorSolucao->commit();
        $this->view->responsavelSolucaoSe->commit();
        $this->view->responsavelSolucaoSeAut->commit();
        $this->view->aeObjetivoEstrategico->commit();
		$this->view->secretariaSolucao->commit();
		$this->view->estrategiaSolucao->commit();
		$this->view->artigoSolucao->commit();
    }

    function carregaSolucaoById($solid) {
        $_SESSION['solid'] = $solid;
        if (!is_int($_SESSION['etpid']))
            $_SESSION['etpid'] = $this->view->etapa->possuiEtapa();

        $this->view->solucao->populateEntity(array('solid' => $solid));
        $this->view->solucao->treatEntityToUser();
        $this->view->tituloSolucao = $this->view->solucao->getTituloSolucao();
    }

    public function resetarSessaoSolucaoAction() {
        $_SESSION['solid'] = null;
        $_SESSION['etpid'] = null;
        $_SESSION['atvid'] = null;
        $_SESSION['acaids'] = null;
        $_SESSION['secid'] = null;
        $_SESSION['estid'] = null;
    }

    public function ordenarAction() {
        $novaOrdem = $this->getPost('novaOrdem');
        $novaOrdem = array_filter($novaOrdem);
        $cont = 0;
        foreach ($novaOrdem as $idSolucao) {
            $idSolucaoArray = explode('_', $idSolucao);

            if ($idSolucaoArray[2] == 'solucao') {
                $cont++;
                $solid = (int) end($idSolucaoArray);
                try {
                    $this->view->solucao = new Model_Solucao(false);
                    $this->view->solucao->alterarOrdem($solid, $cont);
                    $this->view->solucao->commit();
                } catch (Exception $e) {
                    $this->view->solucao->rollback();
                }
            }
        }
        $this->listarAction();
    }


	public function formularioAcoesEstrategicaAction() {
		$temid = $_POST['temid'];
		if(empty($temid)){
			$this->view->where = null;
		}else{
			$this->view->where = " AND temid IN ( {$temid} ) ";
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function formularioMetaSolucaoAction() {
		$temid = $_POST['temid'];
		if(empty($temid)){
			$this->view->where = null;
		}else{
			$this->view->where = " OR temid IN ( {$temid} ) ";
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function formularioIniciativaAction() {
		$temid = $_POST['temid'];
		$obeid = $_POST['obeid'];
        $this->view->where = null;

		if(!empty($temid)){
			$this->view->where .= " AND ini.temid IN ( {$temid} ) ";
		}
        if(!empty($obeid)){
            $this->view->where .= " AND ob.obeid IN ( {$obeid} ) ";
        }

		$this->render(__CLASS__, __FUNCTION__);
	}

	public function formularioObjetivoEstrategicoAction() {
		$temid = $_POST['temid'];
		$mpneid = $_POST['mpneid'];
		if(!empty($temid)){
			$this->view->where = " AND oe.temid IN ( {$temid} ) ";
		}
		if(!empty($mpneid)){
			$this->view->where .=  " AND oepne.mpneid IN ( {$mpneid} ) ";
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function formularioEstrategiaAction() {
		$mpneid = $_POST['mpneid'];
		if(empty($mpneid)){
			$this->view->where = null;
		}else{
			$this->view->where = " AND metid IN ( {$mpneid} ) ";
		}
		$this->render(__CLASS__, __FUNCTION__);
	}

    public function formularioArtigoAction() {
        $temid = $_POST['temid'];
        if(empty($temid)){
            $this->view->where = null;
        }else{
            $this->view->where = " AND temid IN ( {$temid} ) ";
        }
        $this->render(__CLASS__, __FUNCTION__);
    }
}
