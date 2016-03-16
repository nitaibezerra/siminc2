<?php

class ItensComposicaoObras extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.itenscomposicaoobra";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "icoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array('icoid'                => null,
                                       'obrid'                => null,
                                       'itcid'                => null,
                                       'icopercprojperiodo'   => null,
                                       'icopercsobreestrutura'=> null,
                                       'icovlritem'           => null,
                                       'icodtinicioitem'      => null,
                                       'icodterminoitem'      => null,
                                       'icostatus'            => null,
                                       'icodtinclusao'        => null,
                                       'icopercexecutado'     => null,
                                       'icoordem'             => null,
                                       'icovigente'           => null,
                                       'icoid_1'           	  => null,
                                       'itcquantidade'        => null,
                                       'relativoedificacao'   => null,
                                       'umdid'           	  => null,
                                       'croid'           	  => null
	       );

	public function getDadosCabecalho( $icoid ){
		$sql = "SELECT
					itc.itcdesc,
					TO_CHAR(icodtinicioitem, 'DD/MM/YYYY') AS icodtinicioitem,
					TO_CHAR(icodterminoitem, 'DD/MM/YYYY') AS icodterminoitem,
					icovlritem
			    FROM
	                obras2.itenscomposicaoobra ico
	            JOIN
	                obras2.itenscomposicao itc ON itc.itcid = ico.itcid
	            WHERE
	                icoid = {$icoid} AND
	                icostatus = 'A'";
		
		$dados = $this->pegaLinha( $sql );
		return ($dados ? $dados : array());
	}	       

    public function getValorTotalItens($obrid, $relativoEdificacao = 'D'){
        $itens = $this->getItemComposicaoByObra($obrid, $relativoEdificacao);
        $tot = 0;

        if(!$itens)
            return 0;

        foreach($itens as $item){
            if($relativoEdificacao == 'F'){
                $tot += ($item['icovlritem'] * $item['itcquantidade']);
            } else {
                $tot += $item['icovlritem'];
            }
        }


        return $tot;
    }

    public function getItemComposicaoByCronograma($croid, $relativoEdificacao = 'D'){

        $sql = "SELECT
		              ico.icoid,
		              ico.itcid,
		              ico.icodtinicioitem,
		              ico.icodterminoitem,
		              ico.icovlritem,
		              itc.itcdesc,
                              itc.itcordem,
                              ico.icoordem,
                              ( SELECT count(di.ditid)
                                FROM obras2.detalheitem di
                                WHERE di.icoid = ico.icoid
                                AND di.ditstatus = 'A' ) as qtddetalhamento,
                      ico.itcquantidade,
                      cro.croid
	            FROM
	                obras2.itenscomposicaoobra ico
	            JOIN
	                obras2.itenscomposicao itc ON itc.itcid = ico.itcid
	            JOIN obras2.cronograma cro ON cro.croid = ico.croid
	            WHERE
	                cro.croid = {$croid} AND
	                ico.obrid = cro.obrid AND
	                icostatus = 'A' AND
	                relativoedificacao = '{$relativoEdificacao}'
	            ORDER BY
	                icoordem
                        " ;

        return $this->carregar($sql);
    }

	public function getItemComposicaoByObra($obrid, $relativoEdificacao = 'D'){

		$sql = "SELECT
		              ico.icoid,
		              ico.itcid,
		              ico.icodtinicioitem,
		              ico.icodterminoitem,
		              ico.icovlritem,
		              itc.itcdesc,
                              itc.itcordem,
                              ico.icoordem,
                              ( SELECT count(di.ditid) 
                                FROM obras2.detalheitem di 
                                WHERE di.icoid = ico.icoid 
                                AND di.ditstatus = 'A' ) as qtddetalhamento,
                      ico.itcquantidade,
                      cro.croid
	            FROM
	                obras2.itenscomposicaoobra ico
	            JOIN
	                obras2.itenscomposicao itc ON itc.itcid = ico.itcid
	            JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
	            WHERE
	                cro.obrid = {$obrid} AND
	                ico.obrid = cro.obrid AND
	                icostatus = 'A' AND
	                relativoedificacao = '{$relativoEdificacao}'
	            ORDER BY
	                icoordem 
                        " ;

//                ver($sql);
		return $this->carregar($sql);
	}
	
	public function pegaDadosPorObrid( $obrid ){

		$sql = "SELECT
					ico.*,
					itc.*
	            FROM
	                obras2.itenscomposicaoobra ico
	            JOIN
	                obras2.itenscomposicao itc ON itc.itcid = ico.itcid
	            WHERE
	                ico.obrid = {$obrid} AND
	                ico.icostatus = 'A'
	            ORDER BY
	                ico.icoordem";
		$dados = $this->carregar($sql);
		
		return ($dados ? $dados : array());
	}

	public function getAllID($obrid){

		$sql = "SELECT
	                itcid
	            FROM
	                obras2.itenscomposicaoobra i
	            JOIN obras2.cronograma cro ON cro.crostatus = 'A' AND cro.croid = i.croid
	            WHERE
	                cro.obrid = {$obrid};";

		return $this->carregarColuna($sql);
	}

	public function getAllIcoid($obrid, $croid = null){
        $where = " AND c.crostatus = 'A' ";
        if ($croid) {
            $where = " AND i.croid = $croid ";
        }
		$sql = "SELECT
	                i.icoid
	            FROM
	                obras2.itenscomposicaoobra i
	            JOIN obras2.cronograma c ON c.croid = i.croid
	            WHERE
	                c.obrid = {$obrid} $where;";

		return $this->carregarColuna($sql);
	}

	public function desabilitarItens($arChaves, $obrid){

		if(count($arChaves) > 0){
			$sql = "UPDATE
		                obras2.itenscomposicaoobra
		            SET
		                icostatus = 'I'
		            WHERE
		                itcid IN (".implode(',', $arChaves).") AND relativoedificacao = 'D' AND
		                obrid = {$obrid};";

	       $this->executar($sql);
		}
	}

	public function desabilitarItensPeloIcoid($arChaves, $obrid){

		if(count($arChaves) > 0){
			$sql = "UPDATE
		                obras2.itenscomposicaoobra
		            SET
		                icostatus = 'I'
		            WHERE
		                icoid IN (".implode(',', $arChaves).") AND
		                obrid = {$obrid};";

	       $this->executar($sql);
		}
	}

    public function desabilitarItensPeloIcoidECroid($arChaves, $croid){

        if(count($arChaves) > 0){
            $sql = "UPDATE
		                obras2.itenscomposicaoobra
		            SET
		                icostatus = 'I'
		            WHERE
		                icoid IN (".implode(',', $arChaves).") AND relativoedificacao = 'D' AND
		                croid = {$croid};";

            $this->executar($sql);
        }
    }

	public function getIdByNameAndObra($itcdesc, $obrid){
        $sql = "SELECT
                      ico.icoid
                FROM
                    obras2.itenscomposicaoobra ico
                    JOIN ocrs2.crongorama cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
                JOIN
                    obras2.itenscomposicao itc ON itc.itcid = ico.itcid
                                                  AND itcstatus = 'A'
                                                  AND TRIM(UPPER(itcdesc)) = TRIM(UPPER('{$itcdesc}'))
                WHERE
                    ico.obrid = {$obrid} AND
                    cro.obrid = {$obrid} AND
                    ico.icostatus = 'A'";

        return $this->pegaUm( $sql );
	}

	public function getByObraAndItens($obrid, $itcid){
        $sql = "SELECT
                      *,
                      icodtinicioitem AS icodtinicioitembd,
                      icodterminoitem AS icodterminoitembd,
                      to_char(icodtinicioitem, 'dd/mm/YYYY') AS icodtinicioitem,
                      to_char(icodterminoitem, 'dd/mm/YYYY') AS icodterminoitem
                FROM
                    obras2.itenscomposicaoobra ico
                    JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
                JOIN
                    obras2.itenscomposicao itc ON itc.itcid = ico.itcid
                                                  AND itc.itcstatus = 'A'
                                                  AND itc.itcid = {$itcid}
                WHERE
                    ico.obrid = {$obrid} AND
                    cro.obrid = {$obrid} AND
                    ico.icostatus = 'A'";

        $dados =  $this->pegaLinha( $sql );
//dbg($sql);
        return ($dados ? $dados : array());
	}

	public function getByObraAndItensByMacro($obrid, $mceid){

        $sql = "SELECT
                      SUM(icovlritem) AS icovlritem,
                      -- SUM(icopercexecutado) AS icopercexecutado,
                      SUM(icopercexecutado) / (SELECT
												      COUNT(icoid)
												FROM
												    obras2.itenscomposicaoobra ico
												JOIN
												    obras2.itenscomposicao itc ON itc.itcid = ico.itcid
												          AND itc.itcstatus = 'A'
												          AND itc.mceid = {$mceid}
												WHERE
												    ico.obrid = {$obrid} AND
												    ico.icostatus = 'A' ) AS icopercexecutado,
                      to_char(MIN(icodtinicioitem), 'dd/mm/YYYY') AS icodtinicioitem,
                      to_char(MAX(icodterminoitem), 'dd/mm/YYYY') AS icodterminoitem,
                      mceid

                FROM
                    obras2.itenscomposicaoobra ico
                JOIN obrs2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
                JOIN
                    obras2.itenscomposicao itc ON itc.itcid = ico.itcid
                                  AND itc.itcstatus = 'A'
                                  AND itc.mceid = {$mceid}
                WHERE
                    ico.obrid = {$obrid} AND
                    cro.obrid = {$obrid} AND
                    ico.icostatus = 'A'
                GROUP BY
                    mceid;";

        $dados =  $this->pegaLinha( $sql );
        return ($dados ? $dados : array());
	}

	public function getSomaEtapaByObra($obrid, $icoidExceto=null){
        $sql = "SELECT
                    SUM(icovlritem) AS soma
                FROM
                    obras2.itenscomposicaoobra i
                JOIN obras2.cronograma cro ON cro.croid = i.croid  AND cro.crostatus = 'A'
                WHERE
                    i.obrid = {$obrid} AND
                   " . ($icoidExceto ? "icoid NOT IN (" . implode(", ", (array) $icoidExceto) . ") AND" : "") . "
                    icostatus = 'A'";

		$dado = $this->pegaUm( $sql );
		return ($dado ? $dado : 0);
	}

	public function getIdByObra( $obrid ){

		$sql = "SELECT
	                i.icoid
	            FROM
	                obras2.itenscomposicaoobra i
	            JOIN obras2.cronograma cro ON cro.croid =   i.croid AND cro.crostatus = 'A'
	            WHERE
	            	i.icostatus = 'A' AND
	            	cro.obrid = {$obrid} AND
	                i.obrid = {$obrid};";

		return $this->carregarColuna($sql);
	}

}