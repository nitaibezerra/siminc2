<?php

class PrevisaoDetalheItem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.previsaodetalheitem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pdiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'pdiid'         => null,
									  	'ditid'         => null,
									  	'pdivalor'      => null,
									  	'pdimes'        => null,
									  	'pdistatus'     => null,
									  	'pdimetafisica' => null
									  );

    public function desabilitar( $ditid ){
        if ( !$ditid ){
            return false;
        }
        $ditid = (array) $ditid;
        $ditid = implode(', ', $ditid);

        $sql = "UPDATE obras2.previsaodetalheitem SET
                    pdistatus  = 'I'
                WHERE
                    ditid IN(" . $ditid . ")";

        $this->executar( $sql );
    }

    public function getValor($ditid, $ano, $mes){
        $ditid = (array) $ditid;
        $ditid = implode(', ', $ditid);
        $mes = str_pad($mes, 2, "0", STR_PAD_LEFT);

        $sql = "SELECT
                    SUM(pdivalor)
                FROM
                    obras2.previsaodetalheitem
                WHERE
                    pdistatus = 'A'
                    AND ditid IN ($ditid)
                    AND to_char(pdimes, 'YYYY') = '{$ano}'
                    AND to_char(pdimes, 'MM') = '{$mes}'";

        return $this->pegaUm($sql);
    }

    public function getMetaFisica($ditid, $ano, $mes){
        $ditid = (array) $ditid;
        $ditid = implode(', ', $ditid);
        $mes = str_pad($mes, 2, "0", STR_PAD_LEFT);

        $sql = "SELECT
                    pdimetafisica
                FROM
                    obras2.previsaodetalheitem
                WHERE
                    pdistatus = 'A'
                    AND ditid IN ($ditid)
                    AND to_char(pdimes, 'YYYY') = '{$ano}'
                    AND to_char(pdimes, 'MM') = '{$mes}'";

        return $this->pegaUm($sql);
    }

	public function getIdByDetalheItem( $ditid ){
		$where[] = "pdistatus = 'A'";
		$where[] = "ditid = {$ditid}";

		$sql = "SELECT
					pdiid
				FROM
					obras2.previsaodetalheitem
				WHERE
				" . implode(" AND ", $where);

		return $this->carregarColuna($sql);
	}
    
}