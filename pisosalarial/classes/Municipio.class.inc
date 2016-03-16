<?php
/**
 * Modelo de Plano de Carreira
 * @author Wesley Romualdo da Silva
 */
class Municipio extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.municipio";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "muncod" );

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'muncod' => null,
									  	'estuf' => null,
									  	'miccod' => null,
									  	'mescod' => null,
									  	'mundescricao' => null,
									  	'munprocesso' => null,
									  	'muncodcompleto' => null,
									  	'munmedlat' => null,
									  	'munmedlog' => null,
									  	'munhemis' => null,
									  	'munaltitude' => null,
									  	'munmedarea' => null,
									  	'muncepmenor' => null,
									  	'muncepmaior' => null,
									  	'munmedraio' => null,
									  	'munmerid' => null,
									  	'muncodsiafi' => null,
									  	'munpopulacao' => null,
									  	'munpopulacaohomem' => null,
									  	'munpopulacaomulher' => null,
									  	'munpopulacaourbana' => null,
									  	'munpopulacaorural' => null
									  );

	public function listaMunicipios($post = null){

	    if(is_array($post)){
            extract($post);
	    }

	    $stWhere = "";
        if(isset($folhapagamento)){
            $stWhere .= " AND (SELECT count(flp.flpid) FROM pisosalarial.folhapagamento flp
                    INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = flp.pmuid
                    WHERE pi.muncod = mun.muncod
                    AND (SELECT count(*) FROM pisosalarial.folhapagamentoprofissionais ff WHERE flp.flpid = ff.flpid) > 0
                    ) > 0 ";
        }
        if(isset($planocarreira)){
            $stWhere .= " AND (SELECT count(*) FROM pisosalarial.planocarreira plc WHERE plc.muncod = mun.muncod) > 0 ";
        }
        if(isset($gestaorecursos)){
            $stWhere .= " AND (SELECT count(*) FROM pisosalarial.gestaorecurso ger WHERE ger.muncod = mun.muncod) > 0 ";
        }
        if(!empty($municipio)){
            $stWhere .= " AND translate(mun.mundescricao,'áàâãäéèêëíìïóòôõöúùûüÁÀÂÃÄÉÈÊËÍÌÏÓÒÔÕÖÚÙÛÜçÇ','aaaaaeeeeiiiooooouuuuAAAAAEEEEIIIOOOOOUUUUcC')  ilike '%' || translate('{$municipio}','áàâãäéèêëíìïóòôõöúùûüÁÀÂÃÄÉÈÊËÍÌÏÓÒÔÕÖÚÙÛÜçÇ','aaaaaeeeeiiiooooouuuuAAAAAEEEEIIIOOOOOUUUUcC') || '%'";
        }
        if(!empty($estuf)){
            $stWhere .= " AND mun.estuf = '".strtoupper($estuf)."'";
        }

	    $sql = "SELECT
	               '<img id=\"' || mun.muncod || '_' || mun.estuf || '\" style=\"cursor:pointer\" src=\"/imagens/consultar.gif \"
	                border=0 title=\"Visualizar\" class=\"visualiza_piso\">' as acao,
	               mun.estuf,
	               mun.mundescricao,
	               (SELECT count(*) FROM pisosalarial.gestaorecurso ger WHERE ger.muncod = mun.muncod) as gestao,
                   (SELECT count(*) FROM pisosalarial.planocarreira plc WHERE plc.muncod = mun.muncod) as plano,
                   (SELECT count(flp.flpid) FROM pisosalarial.folhapagamento flp
                   INNER JOIN pisosalarial.pisomunicipio mm on flp.pmuid = mm.pmuid
                    WHERE mm.muncod = mun.muncod
                    AND (SELECT count(*) FROM pisosalarial.folhapagamentoprofissionais ff WHERE flp.flpid = ff.flpid) > 0
                    ) as folha
	            FROM {$this->stNomeTabela} mun
	            WHERE mun.muncod IS NOT NULL
	            AND mun.estuf IN ('AL', 'BA', 'CE', 'MA', 'PB', 'PE', 'PI', 'RN', 'AM', 'PA')
	            {$stWhere}
	            GROUP BY mun.muncod, mun.mundescricao, mun.estuf
	            ORDER BY mun.mundescricao";

	    $cabecalho = array("Ações", "UF", "Município", "Gestão de recursos", "Plano de carreira", "Folha pagamento");
        $this->monta_lista($sql,$cabecalho,50,10,false,"center");

	}
}
?>