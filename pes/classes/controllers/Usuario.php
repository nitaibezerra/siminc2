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
class Controller_Usuario extends Abstract_Controller
{
    
    /**
     * 
     */
    public function barraEntidadeUsuarioAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }
    
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
    
    public function selecionarEntidadeAction()
    {
        $usuarioResponsabilidade = new Model_UsuarioResponsabilidade();
        
        $arrPflcod = pegaPerfilGeral();
        $arrUsuarioResponsabilidade = $usuarioResponsabilidade->carregarTudoPorCpfPerfilEntidade($this->user()->cpf, $arrPflcod, $this->getPost('entcodigo'));

        // Se tiver a entidade selecionada ja cadastrada para o usuario e seus perfis ele edita ela, marcando com ultimo acesso.
        if($arrUsuarioResponsabilidade){
            
            // Tirando ultimo acesso de qualquer entidade do usuario.
            $usuarioResponsabilidade->desativarUltimoAcessoPorCpf($this->user()->cpf);
            
            // Ativando como ultimo acesso as entidades de todos os perfils do usuario.
            foreach($arrUsuarioResponsabilidade as $valuesUsuarioResponsabilidade){
                $usuarioResponsabilidade->getEntity();
                $usuarioResponsabilidade->populateEntity($valuesUsuarioResponsabilidade);
                $usuarioResponsabilidade->entity['rpuultimoacesso'] = '1';
                
                $result = $usuarioResponsabilidade->save();
            }
            
        } else {
            
            // Se nao tiver entidade ele salva a entidade selecionada.
            foreach($arrPflcod as $pflcod){
                $usuarioResponsabilidade->getEntity();
                $usuarioResponsabilidade->entity['rpuid'] = '';
                $usuarioResponsabilidade->entity['pflcod'] = $pflcod;
                $usuarioResponsabilidade->entity['usucpf'] = $this->user()->cpf;
                $usuarioResponsabilidade->entity['entcodigo'] = $this->getPost('entcodigo');
                $usuarioResponsabilidade->entity['rpustatus'] = 'A';
                $usuarioResponsabilidade->entity['rpudata_inc'] = 'now()';
                $usuarioResponsabilidade->entity['rpuultimoacesso'] = 'true';
                
                $result = $usuarioResponsabilidade->save();
            }
        }
        
        // Busca orgao, uo e entidade que o usuario teve ultimo acesso para por na barra onde lista.
        $model = new Model_Entidade();
        $entidadeUltimoAcesso = $model->carregarTudoDaUltimaEntidadePorUsuario( $_SESSION['usucpf'] );
        
        // Colocando dados da entidade na sessao.
        $_SESSION['pes']['nome_orgao'] = $entidadeUltimoAcesso['orgnome'];
        $_SESSION['pes']['codigo_orgao'] = $entidadeUltimoAcesso['orgcodigo'];
        $_SESSION['pes']['nome_unidadeorcamentaria'] = $entidadeUltimoAcesso['uornome'];
        $_SESSION['pes']['codigo_unidadeorcamentaria'] = $entidadeUltimoAcesso['uorcodigo'];
        $_SESSION['pes']['nome_entidade'] = $entidadeUltimoAcesso['entnome'];
        $_SESSION['pes']['codigo_entidade'] = $entidadeUltimoAcesso['entcodigo'];
    }
}