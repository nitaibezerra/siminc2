<?php

class Municipio extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.municipio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("muncod");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
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
    );

    public function lista($arInner = array(), $arWhere = array(), $exporta = null, $tipo = '') {
        $link = " '' ";
        //$perfis = pegaPerfil($_SESSION['usucpf']);
        $perfis = pegaArrayPerfil($_SESSION['usucpf']);
    	if (is_array($perfis)) {
            if (count(array_intersect(unserialize(GRUPO_DE_PERFIL_ESTADUAL), $perfis)) > 0 || count(array_intersect(unserialize(GRUPO_DE_PERFIL_MUNICIPAL), $perfis)) > 0) {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreExecucaoOrcamento(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            } else {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            }
        } else if ($perfis !== NULL) {
            if (in_array($perfis, unserialize(GRUPO_DE_PERFIL_ESTADUAL)) || in_array($perfis, unserialize(GRUPO_DE_PERFIL_MUNICIPAL))) {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreExecucaoOrcamento(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            } else {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            }
        }
        
        $today = date('Y-m-d');
        $sqlRestricoes = "SELECT 
                                    CASE 
                                        WHEN count(res.resid) >  0 THEN TRUE
                                        ELSE FALSE
                                    END 
                                FROM par.restricaoentidade re
                                INNER JOIN par.restricaofnde res ON re.resid = res.resid
                                LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
                                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                                WHERE re.inuid = iu.inuid 
                                AND res.resstatus = 'A'
                                AND esd.esdid NOT IN (" . ESDID_CONCLUIDO . ")
                                ";
        if ($exporta == "true") {
            $acaoBtn = "";
            $sqlRestricoes = "";
        } else {
            $acaoBtn = $link . " as acao,";
            $sqlRestricoes = "
				
				,CASE WHEN ({$sqlRestricoes}) THEN 
					'<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreRestricao(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\')\"><img src=\"/imagens/atencao.png\" border=0 title=\"Selecionar\"></a> </center>'
				ELSE
				 	'<center>-</center>'
				END
					as restricoes
			
			";
        }

        if ($tipo == 'doc') {
            $cache = '';
            $cabecalho = array("Selecione", "Código", "Município", "UF", "Situação");
            $acaoBtn = "case when iu.inutermoliberado = false then 
							'<center><input type=\"checkbox\" name=\"libera[]\" value=\"'||m.muncod||'\" class=\"normal\" style=\"margin-top: -1px;\">
								<input type=\"hidden\" id=\"hid_select\" name=\"hid_select[]\" value=\"'||m.muncod||'\">
								<img src=../imagens/report.gif style=cursor:pointer; title=Documentos onclick=\"window.open(\'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&muncod='||m.muncod||'\',\'visualizaDocumento\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>'
						else
							'<center><input type=\"checkbox\" checked=\"checked\" name=\"libera[]\" value=\"'||m.muncod||'\" class=\"normal\" style=\"margin-top: -1px;\">
								<input type=\"hidden\" id=\"hid_select\" name=\"hid_select[]\" value=\"'||m.muncod||'\">
								<img src=../imagens/report.gif style=cursor:pointer; title=Documentos onclick=\"window.open(\'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&muncod='||m.muncod||'\',\'visualizaDocumento\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>'
						end as acao,";
        } else {
            $cache = '3600';
            $cabecalho = array("Ação", "Código", "Município", "UF", "Situação", "Restrições");
        }

        $sql = "SELECT DISTINCT 
        			" . $acaoBtn . " 
					m.muncod, m.mundescricao, m.estuf, CASE WHEN ed.esddsc IS NULL THEN 'Diagnóstico' ELSE CASE WHEN ap.inuid is not null then CASE WHEN ed.esdid = " . WF_ANALISE . " THEN ed.esddsc || ' / Atualização' ELSE ed.esddsc END ELSE ed.esddsc END END as sit
					{$sqlRestricoes}
				FROM territorios.municipio m
				LEFT JOIN par.instrumentounidade 	iu ON iu.mun_estuf = m.estuf AND iu.muncod = m.muncod AND iu.itrid = 2
				LEFT JOIN par.atualizacaopar 		ap ON ap.inuid = iu.inuid
				LEFT JOIN workflow.documento 		d  ON d.docid = iu.docid AND d.tpdid 	= 44 
				LEFT JOIN workflow.estadodocumento 	ed ON ed.esdid = d.esdid
				{$arInner['join_capitais']}
				{$arInner['join_grupo']}
				{$arInner['join_ideb']} 
				{$arInner['join_dadosunidade']}
				" . ( is_array($arWhere) && count($arWhere) ? ' WHERE ' . implode(' AND ', $arWhere) : '' ) . "
				ORDER BY m.mundescricao
				";
        $tamanho = array('5%', '7%', '51%', '10%', '10%', '2%');
        if ($exporta == "true") {
            return $sql;
        } else {
            $this->monta_lista($sql, $cabecalho, 50, 5, 'N', '95%', $par2, 'formlistamunicipio', $tamanho, '', $cache);
        }
    }

    public function descricaoMunicipio($muncod, $boMostraEstuf = true) {
        if ($boMostraEstuf) {
            return $this->pegaUm("SELECT estuf || ' - ' || mundescricao FROM territorios.municipio where muncod = '$muncod' ");
        } else {
            return $this->pegaUm("SELECT mundescricao FROM territorios.municipio where muncod = '$muncod' ");
        }
    }

    public function recuperarUF($muncod) {
        $sql = "SELECT estuf FROM {$this->stNomeTabela} WHERE muncod = '{$muncod}'";
        return $this->pegaUm($sql);
    }

    public function carregarMunicipio() {
        $sql = "SELECT 
                        muncod AS codigo,
                        estuf ||' - '|| mundescricao AS descricao 
                    FROM 
                        {$this->stNomeTabela}
                    ORDER BY 
                        mundescricao;";
        return $this->carregar($sql);
    }
    
    public function carregarMunicipioPorMuncod($muncod) {
        $sql = "SELECT 
                        muncod AS codigo,
                        estuf ||' - '|| mundescricao AS descricao 
                    FROM 
                        {$this->stNomeTabela}
                    WHERE muncod = '{$muncod}'
                    ORDER BY 
                        mundescricao;";
        return $this->pegaLinha($sql);
    }
    
    public function carregarGrupoMunicipio($booSQL){
          $sql = "SELECT 
                    tpmid as codigo, 
                    tpmdsc as descricao
                FROM 
                    territorios.tipomunicipio
                WHERE
                    tpmstatus = 'A' 
                    AND gtmid in ( 1,2 )
                ORDER BY
                    descricao";
          return ($booSQL) ? $sql : $this->carregar($sql);
    }
}
