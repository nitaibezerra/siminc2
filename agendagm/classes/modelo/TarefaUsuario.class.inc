<?php
/**
 * Classe de mapeamento da entidade agendagm.tarefausuario.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade agendagm.tarefausuario.
 *
 * @see Modelo
 */
class Agendagm_Model_Tarefausuario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'agendagm.tarefausuario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'usucpf',
        'trfid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'trfid' => array('tabela' => 'agendagm.tarefa', 'pk' => 'trfid'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'trfid' => null,
    );
    
    public function salvar( $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array() ){
        $trfid          = $this->trfid;
        $array_usucpf   = $this->usucpf;
        
        if( $trfid > 0 ){

            $sql = "DELETE FROM agendagm.tarefausuario WHERE trfid = {$trfid} ";
            $this->executar($sql);

            foreach( $array_usucpf as $usucpf ){
                $sql = "INSERT INTO agendagm.tarefausuario( usucpf, trfid ) VALUES ( '{$usucpf}', {$trfid} ) RETURNING usucpf";
                $result = $this->pegaUm($sql);
            }
        }

        if( $result > 0 ){
            $this->commit();
            return true;
        }else{
            $this->rollback();
            return false;
        }
    }
    
    public function recuperarPorTarefa($trfid){

        $retorno = array();
        if($trfid){
            $sql = "SELECT usucpf FROM agendagm.tarefausuario WHERE trfid = $trfid";
            $dados = $this->carregarColuna($sql);
            $retorno = $dados ? $dados : array();
        }
        return $retorno;
    }
    
}