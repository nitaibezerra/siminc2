<?php

/**
 * Controle responsavel pelas entidades.
 * 
 * @author Equipe simec - Consultores OEI
 * @since  14/05/2013
 * 
 * @name       Entidade
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Entidade extends Abstract_Controller
{
    
    /**
     * Action que lista os orgaos do usuario logado no sistema.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  14/05/2013
     * 
     * @name   listarOrgaoAction
     * @access public
     * @return void
     */
    public function listarOrgaoAction()
    {
        $modelOrgao = new Model_Orgao();
        $orgaoDoUsuario = $modelOrgao->carregarOrgaoPorCpf($this->user()->cpf);
        $this->view->listaOrgao = $orgaoDoUsuario;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    /**
     * Action que lista as unidades orcamentarias do orgao e do usuario logado no sistema.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  14/05/2013
     * 
     * @name   listarUOAction
     * @access public
     * @return void
     */
    public function listarUOAction()
    {
        $modelUO = new Model_UnidadeOrcamentaria();
        $orgcodigo = $this->getPost('orgcodigo');
        $listaUO = $modelUO->carregarUOPorOrgaoECpf($orgcodigo, $this->user()->cpf);
        $this->view->orgcodigo = $orgcodigo;
        $this->view->lista = $listaUO;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    /**
     * Action que lista as entidades do orgaos e do usuario logado no sistema.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  14/05/2013
     * 
     * @name   listarEntidadeAction
     * @access public
     * @return void
     */
    public function listarEntidadeAction()
    {
        $modelEntidade = new Model_Entidade();
        $listaEntidade = $modelEntidade->carregarEntidadePorUOECpf($_POST['uorcodigo'], $this->user()->cpf);
        
        $this->view->uorcodigo = $_POST['uorcodigo'];
        $this->view->lista = $listaEntidade;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    /**
     * Action que salva ou edita a entidade.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  14/05/2013
     * 
     * @name   salvarAction
     * @access public
     * @return void
     */
    public function salvarAction()
    {
        $modelEntidade = new Model_Entidade();
        $modelEntidade->populateEntity($_POST);
        // Se tiver o entcodigo, prepara os dados para edicao
        $entcodigo = $this->getPost( 'entcodigo' );
        $uorcodigo = $this->getPost( 'uorcodigo' );
        
        if($entcodigo){
            $modelEntidade->entity['entdataalteracao'] = 'now()';
            $modelEntidade->entity['entusucpfalteracao'] = $this->user()->cpf;
            
        // Se nao, prepara os dados para insercao.
        } else {
            $modelEntidade->entity['aexano'] = date('Y');
            $modelEntidade->entity['entdatacriacao'] = 'now()';
            $modelEntidade->entity['entusucpfcriacao'] = $this->user()->cpf;
        }
        
        // Verificando se ja existe este codigo fixo cadastrado - Validacao
        $entidadeComMesmoCodigoFixo = $modelEntidade->getByValues(array('entcodigofixo' => $modelEntidade->entity['entcodigofixo']));
        if($entidadeComMesmoCodigoFixo){
            if($modelEntidade->entity['entcodigo'] && $modelEntidade->entity['entcodigo'] != $entidadeComMesmoCodigoFixo['entcodigo'] )
                $modelEntidade->error[] = array("name" => 'entcodigofixo' , "msg" => utf8_encode("Alteração não pode ser realizar pois já existe uma entidade com este código fixo!"));
            elseif(!$modelEntidade->entity['entcodigo'] && $entidadeComMesmoCodigoFixo)
                $modelEntidade->error[] = array("name" => 'entcodigofixo' , "msg" => utf8_encode("Cadastro não pode ser realizar pois já existe uma entidade com este código fixo!"));
        }
        
        // Salva os dados que estao na entity no banco de dados.
        $save = $modelEntidade->save();
        
        // Realiza a exibicao dos dados em json de acordo com o retorno do save.
        if($save){
            
            // Salvando o usuario logado no sistema como responsavel pela entidade.
            $modelUsuarioEntidade = new Model_UsuarioEntidade();

            $parans = array('usucpf' => $this->user()->cpf
                            ,'aexano' => AEXANO
                            ,'entcodigo' => $save
                            ,'uorcodigo' => $uorcodigo
                            ,'aexanouo' => AEXANO);
            // Verifica se ja tem este cara cadastrado como dono da entidade.
            $entityUsuarioEntidadeOld = $modelUsuarioEntidade->getByValues($parans);
            if(!$entityUsuarioEntidadeOld){
                $modelUsuarioEntidade->populateEntity($parans);
                $modelUsuarioEntidade->entity['uendatacriacao'] = 'now()';
                $modelUsuarioEntidade->entity['uenusucpfcriacao'] = $this->user()->cpf;
                $modelUsuarioEntidade->insert();
            }
            
            $return = array('status' => true , 'msg' => MSG001, 'result' => $save);
            
        } else {
            $return = array('status' => false) + $modelEntidade->error[0];
        }
        
        echo simec_json_encode($return); 
        exit;
    }
    
    /**
     * Action que exibe o formulario para cadastro e edicao da entidade.
     * 
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.com>
     * @since  14/05/2013
     * 
     * @name   formularioAction
     * @access public
     * @return void
     */
    public function formularioAction()
    {
        $modelUnidadeFederacao = new Model_UnidadeFederacao();
        
        // Pegando as ufs e exibindo na view para o formulario
        $this->view->uf = $modelUnidadeFederacao->getAll();
        
        $modelEntidade = new Model_Entidade();
        
        // Populando a entidade com o codigo da UO.
        $uorcodigo = $this->getPost('uorcodigo');
        if($uorcodigo){
            $modelEntidade->populateEntity(array('uorcodigo' => $uorcodigo));
            $dataForm = $modelEntidade->entity;
        }

        // Se tiver o codigo da entidade pega a entity para preencher o formulario.
        $entcodigo = $this->getPost('entcodigo');
        if($entcodigo) $dataForm = $modelEntidade->getByValues(array('entcodigo' => $entcodigo));

        // Enviando dados para a view desta action.
        $this->view->dataForm = $dataForm;
        $this->render(__CLASS__, __FUNCTION__);
    }
}
