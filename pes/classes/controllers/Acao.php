<?php

class Controller_Acao extends Abstract_Controller
{

    public function listarOrgaoAction()
    {
        $modelOrgao = new Model_Orgao();
        $orgaoDoUsuario = $modelOrgao->carregarOrgaoPorCpf( $this->user()->cpf );
        $this->view->listaOrgao = $orgaoDoUsuario;
        $this->render( __CLASS__, __FUNCTION__ );
    }

    public function listarUOAction()
    {
        $modelUO = new Model_UnidadeOrcamentaria();
        $listaUO = $modelUO->carregarUOPorOrgaoECpf( $_POST['orgcodigo'], $this->user()->cpf );

        $this->view->orgcodigo = $_POST['orgcodigo'];
        $this->view->lista = $listaUO;
        $this->render( __CLASS__, __FUNCTION__ );
    }

    public function listarEntidadeAction()
    {
        $modelEntidade = new Model_Entidade();
        $listaEntidade = $modelEntidade->carregarEntidadePorUOECpf( $_POST['uorcodigo'], $this->user()->cpf );

        $this->view->uorcodigo = $_POST['uorcodigo'];
        $this->view->lista = $listaEntidade;
        $this->render( __CLASS__, __FUNCTION__ );
    }

    public function alterarSituacaoAcaoAction()
    {
        $modelAcao = new Model_Acao();
        $modelAcao->populateEntity($_POST);

        $resultSave = $modelAcao->save();

        if($resultSave){
            $return = array('status' => true , 'msg' => utf8_encode(MSG009), 'result' => $resultSave);
        } else {
            $return = array('status' => false) + $modelAcao->error[0];
        }

        echo simec_json_encode($return);
        exit;
    }

    public function listarAcaoAction()
    {

        $modelAcao = new Model_Acao();
        $entcodigo = $_SESSION['pes']['codigo_entidade'];
        $tidcodigo = $this->getPost( 'tidcodigo' );

        if($entcodigo && $tidcodigo)
            $values = $modelAcao->carregarAcaoPorEntidadeETipoDespesa($entcodigo, $tidcodigo);
        else $values = null;


        // De acordo com as constantes de acao situacao
        $arrSituacao = array();
        $arrSituacao[] = array('descricao' => 'Concluída','codigo' => 'CO');
        $arrSituacao[] = array('descricao' => 'Cancelada','codigo' => 'CA');
        $arrSituacao[] = array('descricao' => 'Em andamento','codigo' => 'EA');
        $arrSituacao[] = array('descricao' => 'Início atrasado','codigo' => 'IA');
        $arrSituacao[] = array('descricao' => 'Não iniciada','codigo' => 'NI');
        $arrSituacao[] = array('descricao' => 'Termino atrasado','codigo' => 'TA');

        if($this->permission() < 3)
            $save = 'S';
        else 
            $save = 'N';
        
        $this->view->save = $save;
        $this->view->situacao = $arrSituacao;
        $this->view->values = $values;

        $this->render( __CLASS__, __FUNCTION__ );
    }

    public function formularioAction()
    {
        $modelAcao = new Model_Acao();

        $tidcodigo = $this->getPost('tidcodigo');
        $acacodigo = $this->getPost('acacodigo');

        if($acacodigo){
            $dataForm = $modelAcao->getByValues(array('acacodigo' => $acacodigo));
            $dataForm['tidcodigo'] = $tidcodigo;
            $this->dateConvert($dataForm['acadataprevisaoinicio']);
            $this->dateConvert($dataForm['acadataprevisaofim']);
        } else {
            $dataForm = $modelAcao->entity;
            $dataForm['tidcodigo'] = $tidcodigo;
            $dataForm['acacodigo'] = $acacodigo;
        }

        $this->view->permission = $this->permission();
        $this->view->dataForm = $dataForm;
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction()
    {
        $modeEntidade = new Model_Entidade();
        $entcodigo = $_SESSION['pes']['codigo_entidade'];
        $tidcodigo = $this->getPost('tidcodigo');
        $dadosEntidade = $modeEntidade->carregarTudoDaEntidadePorEntidade($entcodigo);

        if(!$dadosEntidade) {
            $returnJson = array('status' => false, 'msg' => utf8_encode('Não pode encontrar entidade com este codigo' . $entcodigo ), 'name' => '' );

            echo simec_json_encode($returnJson);
            exit;
        }

        // Salvando o plano da entidade
        $modelPlanoAcao = new Model_PlanoAcao();

        // Verificando se ja existe plano acao cadastrado
        $params = array(
            'entcodigo' => $entcodigo,
            'tidcodigo' => $tidcodigo,
            'orgcodigo' => $dadosEntidade['orgcodigo'],
            'organo' => $dadosEntidade['organo'],
            'uorcodigo' => $dadosEntidade['uorcodigo'],
        );

        $entityPlanoAcao = $modelPlanoAcao->getByValues($params);

        //Se não tiver plano acao ele insere um novo plano acao
        if(!$entityPlanoAcao){

            $modelPlanoAcao->populateEntity($_POST);

            $modelPlanoAcao->populateEntity($dadosEntidade);

            $modelPlanoAcao->entity['usucpfresponsavel'] = $this->user()->cpf;
            $modelPlanoAcao->entity['usunomeresponsavel'] =  $this->user()->nome;
            $modelPlanoAcao->entity['pladatacriacao'] = 'now()';
            $modelPlanoAcao->entity['plausucpfcriacao'] = $this->user()->cpf;

            $placodigo = $modelPlanoAcao->save();
        } else {
            $placodigo = $entityPlanoAcao['placodigo'];
        }

        // Cadastro acao
        $modelAcao = new Model_Acao();
        $modelAcao->populateEntity($_POST);
        $modelAcao->entity['placodigo'] = $placodigo;
        $modelAcao->entity['acasituacao'] = ACAO_NAO_INICIADA;

        if($modelAcao->entity['acacodigo']){
            $modelAcao->entity['acadataalteracao'] = 'now()';
            $modelAcao->entity['acausucpfalteracao'] = $this->user()->cpf;
        } else {
            $modelAcao->entity['acadatacriacao'] = 'now()';
            $modelAcao->entity['acausucpfcriacao'] = $this->user()->cpf;
        }

        // Validando data de inicio com a data de fim.
        $datesIsValid = $this->datesIsValid($modelAcao->entity['acadataprevisaoinicio'], $modelAcao->entity['acadataprevisaofim']);
        if(!$datesIsValid) {
            $returnJson = array('status' => false, 'msg' => utf8_encode('Data de inicío prevista não pode ser maior que a data de fim prevista!' ), 'name' => 'acadataprevisaofim' );
            echo simec_json_encode($returnJson);
            exit;
        }

        $resultSave = $modelAcao->save();

        if($resultSave){
            $return = array('status' => true , 'msg' => MSG001, 'result' => $resultSave);
        } else {
            $return = array('status' => false) + $modelAcao->error[0];
        }

        echo simec_json_encode($return);
        exit;
    }

    public function excluirAction()
    {
        $modelAcao = new Model_Acao();

        $acacodigo = $this->getPost('acacodigo');
        if($acacodigo){
            if($modelAcao->delete($acacodigo)){
                $return = array('status' => true , 'msg' => utf8_encode(MSG006));
            } else {
                $return = array('status' => false , 'msg' => utf8_encode(MSG007));
            }

            echo simec_json_encode($return);
            exit;
        }
    }

    public function listarSugestoesAction()
    {
        global $db;

        $tidcodigo = $_REQUEST['tidcodigo'] ? $_REQUEST['tidcodigo'] : 0;

        $sqlAcao = "'<input type=\"radio\" name=\"aprcodigo\" descricao=\"' || aprdescricao || '\" class=\"selecionar-sugestao\" />'";
        
        $sql = "select
                    {$sqlAcao} as acao,
                    aprdescricao
                from pes.pesacaopadronizada
                where tidcodigo = '$tidcodigo'
                order by aprdescricao";

        $cabecalho = array( "Ação", "Descrição");
        $db->monta_lista_simples($sql, $cabecalho, 25, 10, 'N');

        echo '
            <script language="javascript" type="text/javascript">
                jQuery(function(){
                    jQuery(".selecionar-sugestao").click(function(){
                        jQuery("#acadescricaoacao").val($(this).attr("descricao"));
                        jQuery("#dialog-sugestao").html("");
                        jQuery("#dialog-sugestao").dialog( "close" );
                    });
                });
            </script>';

        exit;
    }

}