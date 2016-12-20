<?php


class Ted_Model_UnidadeGestora extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.unidadegestora';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('ungcod');

    /**
     * @var TermoCooperacao Entity
     */
    //protected $stTermoCompromisso;

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ungcod' => NULL,
        'ungdsc' => NULL,
        'ungstatus' => NULL,
        'unicod' => NULL,
        'unitpocod' => NULL,
        'uniid' => NULL,
        'ungabrev' => NULL,
        'ungcodsetorialorcamentaria' => NULL,
        'ungcodsetorialauditoria' => NULL,
        'ungcodsetorialcontabil' => NULL,
        'ungcodsetorialfinanceira' => NULL,
        'ungcodpolo' => NULL,
        'ungdescentralfinancsit' => NULL,
        'orgcod' => NULL,
        'ungcnpj' => NULL,
        'ungendereco' => NULL,
        'ungfone' => NULL,
        'muncod' => NULL,
        'ungemail' => NULL,
        'ungbairro' => NULL,
        'ungcep' => NULL,
        'ungnumddfone' => NULL,
        'ungnumddfax' => NULL,
        'ungfonefax' => NULL,
        'gescod' => NULL,
        'podelancarcredito' => NULL
    );

    public function __construct()
    {
        //$this->stTermoCompromisso = new Ted_Model_TermoExecucaoDescentralizada();
    }
    
    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
    );
    
    /**
     * Valida campos obrigatorios no objeto populado
     *
     * @author Sávio Resende - Copiador por Lindalberto Filho
     * @return bool
    */
    public function validaCamposObrigatorios()
    {
    	foreach ($this->arAtributosObrigatorios as $chave => $valor) {
    		if (!isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]))
    			return false;
    	}
    	return true;
    }

    /**
     * @return array|null|void
     */
    public function pegaListaProponente($emCadastramento = true)
    {
        //se não for, traz todas as ug's associadas ao termo
        $responsabilidade = new Ted_Model_Responsabilidade();
        $ungcods = $responsabilidade->filtroUG();

        if ($ungcods) $andWhere = " and ungcod in ({$ungcods})";
        else $andWhere = '';

        $strSQL = sprintf("
            SELECT
                ungcod as codigo,
                ug.ungcod || ' - ' || ungdsc as descricao
            FROM {$this->stNomeTabela} ug
                inner join public.unidade u ON u.unicod = ug.unicod
            WHERE ungstatus='A'
            %s
            ORDER BY 2
        ", $andWhere);

//        ver($strSQL, d);
        $list = $this->carregar($strSQL);
        $options = array();
        if ($list) {
            foreach($list as $item) {
                $options[$item['codigo']] = $item['descricao'];
            }
        }

        return ($options) ? $options : array();
    }

    /**
     * @return array|null|void
     */
    public function pegaListaConcedente()
    {
        $strSQL = "
            SELECT
                ungcod as codigo,
                ungcod || ' - ' || ungabrev||' / '||ungdsc as descricao
            FROM {$this->stNomeTabela}
            WHERE ungstatus = 'A'
            and unicod IN(". UNIDADES_OBRIGATORIAS. ")
            ORDER BY 2";

        $list = $this->carregar($strSQL);
        $options = array();
        if ($list) {
            foreach($list as $item) {
                $options[$item['codigo']] = $item['descricao'];
            }
        }

        return ($options) ? $options : array();
    }

    /**
     * 
     * @return array|null|void
     */
    public function pegaListaUg($ungcod = array())
    {
        $where = (count($ungcod)) ? ' AND '.implode(' AND ', $ungcod) : '';

    	$strSQL = "
            SELECT DISTINCT
                ung.ungcod as codigo,
                uni.unicod || ' - ' || ung.ungdsc as descricao
            FROM
                public.unidadegestora ung
            INNER JOIN unidade uni
                ON uni.unicod = ung.unicod
            WHERE
                ungstatus = 'A'
                {$where}
    	";

    	$list = $this->carregar($strSQL);
    	$options = array();
    	if ($list) {
    		foreach($list as $item) {
    			$options[$item['codigo']] = $item['descricao'];
    		}
    	}
    	 
    	return ($options) ? $options : array();
    }
    

    /**
     * @param $ungcod
     * @return array|bool|null|void
     */
    public function pegaUnidade($ungcod)
    {
        $strSQL = "
            SELECT	ungcod,
                    ungcnpj,
                    ungcod as ungdsc,
                    ungdsc as descricao,
                    ungdsc as razao,
                    ungendereco,
                    ungbairro,
                    mun.estuf,
                    est.estdescricao as estado,
                    mun.muncod,
                    mun.mundescricao as municipio,
                    ungcep,
                    ungfone,
                    ungemail,
                    gescod,
                    unicod
            FROM {$this->stNomeTabela} ung
            LEFT JOIN territorios.municipio mun ON mun.muncod = ung.muncod
            LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
            WHERE ungstatus = 'A' AND ungcod = '{$ungcod}'
        ";

        $return = $this->pegaLinha($strSQL);
        return ($return) ? $return : null;
    }

    /**
     * Captura lista de responsáveis pela politica do FNDE
     */
    public function pegaListaResponsavelPolitica()
    {
    	$sql = "SELECT
			dircod || '_dircod' AS codigo,
			''||ug.ungabrev||' / ' || dirdsc AS descricao
			FROM public.unidadegestora ug
			INNER JOIN ted.diretoria d ON d.ungcod = ug.ungcod
			WHERE ungstatus='A' AND dirstatus = 'A'
			AND d.dircod IN (38,39,41,42,43,58)
    	
			UNION ALL
    	
			SELECT
			ungcod || '_ungcod' AS codigo,
			ungcod || ' - ' || ungabrev||' / '||ungdsc AS descricao
			FROM public.unidadegestora
			WHERE ungstatus = 'A'
			AND ungcod IN ('150019','150028','150016')";
    	
    	$list = $this->carregar($sql);
    	$options = array();
    	if ($list) {
    		foreach($list as $item) {
    			$options[$item['codigo']] = $item['descricao'];
    		}
    	}

    	return ($options) ? $options : array();
    }
    
    public function atualizaDadosUnidadeGestora($dados)
    {
    	$this->popularDadosObjeto($dados);
    	return $this->atualizarUnidadeGestora();
    }
    
    /**
     * Atualiza Unidade Gestora
     *
     * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    private function atualizarUnidadeGestora()
    {
    	if ($this->validaCamposObrigatorios()) {
    		$this->alterar();
    		return $this->commit();
    	}
    
    	return false;
    }

    /**
     * @param $unicod
     * @return array|bool|void
     */
    public function get($unicod)
    {
        $strSQL = "
            SELECT
                ung.ungcod as id,
                uni.unicod,
                uni.unidsc,
                ung.ungabrev,
                ung.gescod,
                ung.ungcod,
                ung.ungdsc,
                rpl.cpf,
                rpl.nome,
                rpl.email
            FROM {$this->stNomeTabela} ung
            LEFT JOIN ted.representantelegal rpl ON ung.ungcod = rpl.ug
            LEFT JOIN public.unidade uni ON ung.unicod = uni.unicod
            WHERE ung.ungcod = '{$unicod}'
        ";

        return $this->pegaLinha($strSQL);
    }

    /**
     * @param array $post
     * @return bool
     */
    public function save(array $post)
    {
        $this->popularDadosObjeto($post);
        $this->arAtributos['ungcod'] = "'{$this->arAtributos['ungcod']}'";

        //ver($post);
        //ver($this->arAtributos, d);

        if (!$this->pegaLinha("SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '{$post['ungcod']}'")) {
            return $this->salvarDadosUnidadeGestora();
        } else {
            return $this->atualizarUnidadeGestora();
        }
    }

    /**
     * Cadastrar Uo
     *
     * @return bool  - retorna 'false' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    public function salvarDadosUnidadeGestora()
    {
        if ($this->validaCamposObrigatorios()) {
            $this->arAtributos['ungcod'] = $this->inserir();
            return ($this->commit()) ? $this->arAtributos['ungcod'] : false;
        }

        return false;
    }

    /**
     * @param array $arCamposNulo
     * @return bool|int|string|void
     */
    public function inserir($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach( $this->arAtributos as $campo => $valor ){
            //if( $campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                $arCampos[]  = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm( $sql );
        }
    }
    
}