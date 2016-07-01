<?php

class SistemaRepositorio extends Modelo
{

    /**
     * Recupera todos os sistemas ativos
     * 
     * @return array
     */
    function recuperaTodosAtivos() {
        $sql       = "select sidid, sidabrev, siddescricao from demandas.sistemadetalhe where sidstatus = 'A'	order by siddescricao";
        $resultSet = parent::carregar( $sql );
        $lista     = array( );
        if ( $resultSet != null ) {
            foreach ( $resultSet as $linha ) {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
    }

    /**
     * Recupera todos os sistemas que não possuem inventário
     * 
     * @return array  
     */
    public function recuperaNaoInventariados() {
        
        $sql = "SELECT sis.sidid, sis.sidabrev, sis.siddescricao
                FROM demandas.sistemadetalhe sis 
                WHERE sis.sidstatus = 'A'
                AND sis.sidid NOT IN ( SELECT sidid 
                                        FROM inventario.tb_simec_inventario inv 
                                        WHERE inv.st_inventario = 'A' )
                ORDER BY sis.siddescricao";

        $resultSet = parent::carregar( $sql );
        $lista     = array( );
        if ( $resultSet != null ) {
            foreach ( $resultSet as $linha ) {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        
        return $lista;
    }
    
    /**
     * Recupera todos os sistemas que possuem inventário
     * @return type 
     */
    public function recuperarInventariados() {
        
        $sql = "SELECT sis.sidid, sis.sidabrev, sis.siddescricao
                    FROM demandas.sistemadetalhe sis 
                    INNER JOIN inventario.tb_simec_inventario inv
                        ON sis.sidid = inv.sidid
                    WHERE sis.sidstatus     = 'A'
                    AND inv.st_inventario   = 'A'	
                    ORDER BY sis.siddescricao";

        $resultSet = parent::carregar( $sql );
        $lista     = array( );
        if ( $resultSet != null ) {
            foreach ( $resultSet as $linha ) {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        
        return $lista;
    }

    function recuperaPeloId( $idSistema ) {
        $sql = "select sidid, sidabrev, siddescricao from demandas.sistemadetalhe where sidstatus = 'A'	and sidid = $idSistema order by siddescricao";
        try {
            $linha   = parent::pegaLinha( $sql );
            $sistema = $this->montaObjeto( $linha );
            return $sistema;
        } catch ( Exception $e ) {
            return null;
        }
    }

    function recuperePeloIdSolicitacao( $idSolicitacao ) {
        $sql = "select sd.sidid, sd.sidabrev, sd.siddescricao 
			from demandas.sistemadetalhe sd
			join fabrica.solicitacaoservico ss
				on ss.sidid = sd.sidid
			where ss.scsid = $idSolicitacao order by siddescricao";
        try {
            $linha   = parent::pegaLinha( $sql );
            $sistema = $this->montaObjeto( $linha );
            return $sistema;
        } catch ( Exception $e ) {
            return null;
        }
    }

    private function montaObjeto( $linha ) {
        $sistema = new Sistema();
        $sistema->setId( $linha['sidid'] );
        $sistema->setSigla( $linha['sidabrev'] );
        $sistema->setDescricao( $linha['siddescricao'] );
        return $sistema;
    }

}