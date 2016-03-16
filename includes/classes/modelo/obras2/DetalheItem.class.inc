<?php

class DetalheItem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.detalheitem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ditid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'ditid'             => null,
									  	'icoid'             => null,
									  	'ditdsc'            => null,
									  	'ditpercentual'     => null,
									  	'ditvalor'          => null,
									  	'ditdatainicio'     => null,
									  	'ditdatatermino'    => null,
									  	'ditstatus'         => null,
									  	'ditordem'          => null,
                                        'ditpercentual'     => null,
                                        'ditpercexecutado'  => null,
                                        'ditidpai'          => null,
                                        'ditmetafisica'     => null,
                                        'umcid'             => null
									  );

	public function getDadosCabecalho( $ditid ){
		$sql = "SELECT
                    f.*
                FROM(
                        SELECT
                          dtipai.ditid,
                          dtipai.ditdsc,
                          TO_CHAR(dtipai.ditdatainicio, 'DD/MM/YYYY') AS ditdtinicioitem,
                          TO_CHAR(dtipai.ditdatatermino, 'DD/MM/YYYY') AS ditdtterminoitem,
                          dtipai.ditvalor
                        FROM
                            obras2.detalheitem dti
                        JOIN
                            obras2.detalheitem dtipai ON dtipai.ditid = dti.ditidpai AND
                            							 dtipai.ditstatus = 'A'
                        WHERE
                        	dti.ditstatus = 'A' AND
                            dti.ditid = {$ditid}
                    UNION
                        SELECT
                          dti.ditid,
                          dti.ditdsc,
                          TO_CHAR(dti.ditdatainicio, 'DD/MM/YYYY') AS ditdtinicioitem,
                          TO_CHAR(dti.ditdatatermino, 'DD/MM/YYYY') AS ditdtterminoitem,
                          dti.ditvalor
                        FROM
                            obras2.detalheitem dti
                        WHERE
                        	dti.ditstatus = 'A' AND
                            dti.ditid = {$ditid}
                    ) AS f
                ORDER BY
                    f.ditid";
		
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}	       

	public function getDados( array $where = array(), $coluna = null ){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		$where[] = "dit.ditstatus = 'A'";


		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					obras2.detalheitem dit
				WHERE
				" . implode(" AND ", $where) . "
				ORDER BY
				    ditordem";

		return $this->carregar($sql);
	}

	public function getAllID( $icoid ){
		$where[] = "dit.ditstatus = 'A'";
		$where[] = "dit.icoid = {$icoid}";

		$sql = "SELECT
					ditid
				FROM
					obras2.detalheitem dit
				WHERE
				" . implode(" AND ", $where);

		return $this->carregarColuna($sql);
	}

    public function desabilitar( $ditid ){
        if ( !$ditid ){
            return false;
        }
        $ditid = (array) $ditid;
        $ditid = implode(', ', $ditid);

        $sql = "UPDATE obras2.detalheitem SET
                    ditstatus  = 'I'
                WHERE
                    ditid IN(" . $ditid . ")";

        $this->executar( $sql );
    }

    public function getIdAllChildren( $ditidPai ){
        $sql = "SELECT
                    ditid
                FROM
                    obras2.detalheitem
                WHERE
                    ditidpai = {$ditidPai}
                    AND ditstatus = 'A'";

        $dados = $this->carregarColuna($sql);
        return (is_array($dados) ? $dados : array());
    }

    public function getIdByNameAndEtapaAndIdPai($ditdsc, $icoid, $ditidpai){
        $ditidpaiWhere = " AND " . ($ditidpai ? " ditidpai = {$ditidpai} " : " ditidpai IS NULL ");
        $sql = "SELECT
                    ditid
                FROM
                    obras2.detalheitem
                WHERE
                    ditdsc = '{$ditdsc}'
                    AND icoid = {$icoid}
                    {$ditidpaiWhere}
                    AND ditstatus = 'A'";

        return $this->pegaUm( $sql );
    }
}