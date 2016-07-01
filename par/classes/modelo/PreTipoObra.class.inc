<?php

class PreTipoObra extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.pretipoobra";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'ptoid' => null,
									  	'ptodescricao' => null,
									  	'ptostatus' => null,
									  );


	public function recuperaClassificacaoCarga( $preid ){
		if( $preid != '' ){
			return $this->pegaUm("SELECT ptoclassificacaoobra
								  FROM obras.preobra p
								  INNER JOIN obras.pretipoobra t ON p.ptoid = t.ptoid
								  WHERE p.preid = $preid AND p.precarga = true");
		}
	}

	public function testaG1($preid){
		if( $preid != '' && $_SESSION['par']['muncod'] != '' ){

			return $this->pegaUm("SELECT true
								  FROM obras.preobra p
								  INNER JOIN territorios.municipio m ON m.muncod = p.muncodpar
								  INNER JOIN territorios.muntipomunicipio t ON t.muncod = m.muncod
								  WHERE p.preid = $preid -- AND p.precarga = true
								  	AND t.tpmid IN (150,163)");


		}else{
			return true;
		}
	}

	public function recuperarTiposObraAtivas( $tooid )
	{

		if($_SESSION['par']['preid']){
			$sql = "SELECT ptoclassificacaoobra
					FROM obras.preobra   p
					INNER JOIN obras.pretipoobra pt ON pt.ptoid = p.ptoid
					where preid = ".$_SESSION['par']['preid'];

			$ptoclassificacaoobra = $this->pegaUm($sql);

		}

		$ptoclassificacaoobra = $ptoclassificacaoobra ? $ptoclassificacaoobra : $_SESSION['par']['tipo'];

		switch ($ptoclassificacaoobra){
			case 'Q':
				$filtroTipo = "AND ptoclassificacaoobra = 'Q'";
				//$filtroTipo = 'AND ptoid in (5,9)';
				break;
			case 'C':
				$filtroTipo = "AND ptoclassificacaoobra = 'C'";
				//$filtroTipo = 'AND ptoid in (4,8,10)';
				break;
			case 'P':
				$filtroTipo = "AND ptoclassificacaoobra = 'P'";
				break;
			default:
				$classificacao = $this->recuperaClassificacaoCarga($_SESSION['par']['preid']);
				if( $classificacao != '' ){
					$filtroTipo = "AND ptoclassificacaoobra = '$classificacao'";
				}else{
					$filtroTipo = '';
				}
				break;
		}


		$aryFiltroPtoid = array();

		if(!$_SESSION['par']['preid']){
			$aryFiltroPtoid = array('42','43');
		}

		if(verificaGrupoMunicipioMUNCOD($_SESSION['par']['muncod'], true)){
			$aryFiltroPtoid[] = "1";
		}

		$filtroPtoid = implode(",", $aryFiltroPtoid);

		if(!$_SESSION['par']['preid']){
			$novoproinfancia = "AND ptoid IN ({$filtroPtoid})";
		} else {
			$novoproinfancia = "";
		}

		if(!$this->testaG1($_SESSION['par']['preid'])){
			//$filtroTipo .= ' AND ptoid = 1 ';
			$filtroTipo .= "and ptoid not in ( select ptoid from obras.pretipoobra where ptostatus = 'A' and ptoprojetofnde = false and tooid = 1)";
		}
		$esfera = $_SESSION['par']['muncod'] || $_SESSION['par']['estuf'] == 'DF' ? 'M' : 'E';
		$retorno = Array('codigo'=>'','descricao'=>'');

		if($tooid){
			$sql = "SELECT
						ptoid as codigo,
						ptodescricao as descricao
					FROM {$this->stNomeTabela}
					WHERE
						ptostatus = 'A'
						--AND ptocategoria IS NULL
						$filtroTipo
						$novoproinfancia
						AND tooid = $tooid
						AND ptoesfera in ('T','{$esfera}')
					ORDER BY ptodescricao";
//		dbg($sql);
			$retorno = $this->carregar($sql);
		}

		return $retorno ? $retorno : Array('codigo'=>'','descricao'=>'');
	}
        
        public function carregarPreTipoObra($booSQL){
            $sql = "SELECT DISTINCT
                        pto.ptoid as codigo,
                        pto.ptodescricao as descricao
                    FROM
                        {$this->stNomeTabela} pto
                    INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid
                    WHERE
                        pto.tooid = 1
                    ORDER BY 2 desc";
            return ($booSQL) ? $sql : $this->carregar($sql);
        }
        
        public function carregarPreTipoObraClassificacao($booSQL){
            $sql = "SELECT DISTINCT
                        ptoclassificacaoobra as codigo, 
                        ptodescricao as descricao
                    FROM
                        {$this->stNomeTabela} pto
                    WHERE ptostatus = 'A'";
            return ($booSQL) ? $sql : $this->carregar($sql);
        }

}