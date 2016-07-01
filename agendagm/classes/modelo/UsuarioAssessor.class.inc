<?php
/**
 * Classe de mapeamento da entidade agendagm.usuarioassessor.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade agendagm.usuarioassessor.
 *
 * @see Modelo
 */
class Agendagm_Model_UsuarioAssessor extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'agendagm.usuarioassessor';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'usucpf',
        'ptaid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'ptaid' => array('tabela' => 'agendagm.pauta', 'pk' => 'ptaid'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'ptaid' => null,
    );

    public function salvar( $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array() ){

        $ptaid          = $this->ptaid;
        $array_usucpf   = $this->usucpf;

        if( $ptaid > 0 ){

            $sql = "DELETE FROM agendagm.usuarioassessor WHERE ptaid = {$ptaid} ";
            $this->executar($sql);

            foreach( $array_usucpf as $usucpf ){
                $sql = "INSERT INTO agendagm.usuarioassessor( usucpf, ptaid ) VALUES ( '{$usucpf}', {$ptaid} ) RETURNING usucpf";
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

    public function recuperarPorPauta($ptaid){

        $retorno = array();
        if($ptaid){
            $sql = "SELECT usucpf FROM agendagm.usuarioassessor WHERE ptaid = $ptaid";
            $dados = $this->carregarColuna($sql);
            $retorno = $dados ? $dados : array();
        }
        return $retorno;
    }
}