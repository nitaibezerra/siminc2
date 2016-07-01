<?php

class InventarioRepositorio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "inventario.tb_simec_inventario";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "co_inventario" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_inventario'      => null,
        'dt_cadastro'        => null,
        'dt_encerramento'    => null,
        'qtd_pf_total'       => null,
        'sidid'              => null,
        'usucpf'             => null,
        'usucpfencerramento' => null,
        'st_inventario'      => null
    );
    
    public function setAtributos( array $atributos )
    {
        $this->arAtributos = $atributos;
    }

    public function recupereTodosPorIdSistema( $idSistema ) {
        $sql       = "SELECT * 
                        FROM inventario.tb_simec_inventario where sidid = $idSistema";
        $resultSet = $this->carregar( $sql );
        $lista     = array( );

        if ( $resultSet != null ) {
            foreach ( $resultSet as $linha ) {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }
    
    /**
     * Verifica se existe algum inventário cadastrado e ativo para um 
     * determinado sistema
     * 
     * @param int $sidid - Código do sistema a ser verificado
     * @return bool 
     */
    public function possuiInventarioAtivo($sidid) 
    {
        
        $sql    = "SELECT COUNT( co_inventario)  as total_inventario_ativo
                   FROM inventario.tb_simec_inventario inv
                   WHERE inv.sidid  = {$sidid}
                   AND st_inventario = 'A'";
                   
       $inventario = $this->pegaUm($sql);
       
       return (bool) $inventario['total_inventario_ativo'];
    }

    private function montaObjeto( $linha ) {
        $inventario = new Inventario();
        $inventario->setId( $id );
        $inventario->setSistema( new Sistema( $sistema ) );
        $inventario->setDataCadastro( new DateTime( $dataCadastro ) );
        $inventario->setDataEncerramento( new DateTime( $dataEncerramento ) );
        $inventario->setSituacao( $situacao );
        $inventario->setCpfCadastro( $cpfCadastro );
        $inventario->setCpfEncerramento( $cpfEncerramento );
        $inventario->setQtdePF( $qtdePF );
        return $inventario;
    }

    public function cadastrar( array $dados ) {
        $campos  = implode( ', ', array_keys( $dados ) );
        $valores = implode( ', ', $dados );

        $sql = "INSERT INTO inventario.tb_simec_inventario ({$campos})
                    VALUES ({$valores}) RETURNING co_inventario";

        $coInventario = $this->pegaUm( $sql );
        return $coInventario;
    }

}