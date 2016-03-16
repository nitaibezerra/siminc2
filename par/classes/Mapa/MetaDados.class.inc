<?php

/**
 * Metadados - cores, infromações extras...
 */
Interface MetaDadosInterface{
	public function montaDado( $classeDado );
}
/**
 * @internal Abstract Factory
 */
class MetaDados implements MetaDadosInterface
{
	public $estuf = array();
	public $muncod = array();
    public $tpmid = array();
	public $orgid;

	protected $metaDados = array();
	protected $Dado;

	/**
	 * @param string $classeDado
	 */
	public function montaDado( $classeDado ){
		$this->Dado = new $classeDado();
		
		if( !empty($this->estuf) ) $this->Dado->estuf = $this->estuf;
		if( !empty($this->muncod) ) $this->Dado->muncod = $this->muncod;
        if( !empty($this->tpmid) ) $this->Dado->tpmid = $this->tpmid;
		if( !empty($this->orgid) ) $this->Dado->orgid = $this->orgid;

		$this->Dado->carregaDado();
	}

	/**
	 * Aplica Cores aos Municípios
	 *
	 * @param PoligonosInterface $Poligonos
	 * @return void
	 * @internal exige que poligonos carregados tenham carregado municípios
	 * @todo como validar que os poligonos representam estados ou municipios?
	 */
	public function aplicaCoresAosMunicipios( PoligonosInterface $Poligonos ){

		$Poligonos->poligono = $this->aplicaCores( 'muncod', $Poligonos );

	}

	/**
	 * Aplica Cores aos Estados
	 *
	 * @param PoligonosInterface $Poligonos
	 * @return void
	 * @internal exige que poligonos carregados tenham carregado estados
	 */
	public function aplicaCoresAosEstados( PoligonosInterface $Poligonos ){
		
		$Poligonos->poligono = $this->aplicaCores( 'estuf', $Poligonos );

	}

	/**
	 * Aplica Cores
	 *
	 * @param string $dimensao ('estuf',...)
	 * @return void
	 */
	public function aplicaCores( $dimensao, PoligonosInterface $Poligonos ){

		if( !is_array(json_decode($Poligonos->poligono)) ) return false;

		$poligonosDecodificados = json_decode( $Poligonos->poligono );

		if( !$poligonosDecodificados[0]->$dimensao ) return false;

		if( count($poligonosDecodificados) > 0 )
		foreach ($poligonosDecodificados as $chave => $valor) {
			
			if( count($this->Dado->dado) > 0 && is_array($this->Dado->dado) )
			foreach ($this->Dado->dado as $chave2 => $valor2) {
				
				if( $valor->$dimensao == $valor2[$dimensao] && $valor->cor != $valor2['cor'] ){
					$poligonosDecodificados[ $chave ]->cor = $valor2['cor'];
					$poligonosDecodificados[ $chave ]->situacao = (string) utf8_encode($valor2['situacao']);
				}

			}

		}

		return simec_json_encode( $poligonosDecodificados);

	}
}

/**
 * Interface dos Dados
 */
Interface DadosInterface{
	public function carregaDado();
}
/**
 * Classe abstrata para funções que sejam 
 * aplicáveis a todos os dados de forma semelhante
 */
 class DadosAbstract
 {
 	public $estuf = array();
	public $orgid = array();

	public $dado;
 }

/**
 * Dado - Situa��o Mapa Base Nacional Comum (Munic�pios por Tipo)
 */
class DadoSituacaoMunPorTipo extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;

        $sql = "select *,
	case
		when situacao = 'Sim' then 'green'
		when situacao = 'N�o' then 'red'
		else 'white'
	end as cor
 from (
 select mun.muncod as muncod, mun.estuf as estuf,  case when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'Sim'
when bncid is null then 'N�o respondeu'
else  'N�o' end as situacao
           from territorios.municipio mun
                  inner join par.instrumentounidade iu on mun.muncod = iu.muncod
                  inner join territorios.muntipomunicipio munt on munt.muncod = mun.muncod
                   left join par.basenacionalcomum bn on iu.inuid = bn.inuid
                   " . (($this->tpmid!=''&&count($this->tpmid)>0)?" WHERE munt.tpmid IN ( '". (implode( "','",  $this->tpmid )) ."' ) ":"") . "
               --WHERE MUN.ESTUF IN ( '". (implode( "','", $this->estuf )) ."' )

) as cor order by cor";
        $this->dado = $db->carregar( $sql );
    }
}
/**
 * Dado - Situação Assessoramento
 */
class DadoSituacaoAssessoramento extends DadosAbstract implements DadosInterface
{
	/**
	 * Carrega metadados da Situação de Assessoramento
	 * 
	 * @return void
	 * @internal carrrega $this->metaDados
	 * @todo buscar da Modelo
	 */
	public function carregaDado(){
		global $db;

//		$sql = "
//			SELECT 
//				s.stacor as cor, 
//				CASE 
//					WHEN s.stacod IS NULL THEN 'Não Cadastrado' 
//					ELSE s.stadsc 
//				END as situacao, 
//				a.muncod as muncod, 
//				a.estuf as estuf 
//			FROM sase.assessoramento a 
//			INNER JOIN workflow.documento d ON d.docid = a.docid
//			INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid 
//			WHERE 1=1 
//			".( (is_array($this->estuf))?" AND a.estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
                				$sql = "select *,
	case 
		when situacao = 'Sim' then 'green'
		when situacao = 'N�o' then 'red'
		else 'white'
	end as cor
 from (
 select mun.muncod as muncod, mun.estuf as estuf,  case when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'Sim' 
when bncid is null then 'N�o respondeu'
else  'N�o' end as situacao
           from territorios.municipio mun 
                  inner join par.instrumentounidade iu on mun.muncod = iu.muncod
                  inner join territorios.muntipomunicipio munt on munt.muncod = mun.muncod
                   left join par.basenacionalcomum bn on iu.inuid = bn.inuid
                   WHERE 1=1
                   " . (($this->estuf!=''&&count($this->estuf)>0)?" AND MUN.ESTUF IN ( '". (implode( "','",  $this->estuf )) ."' ) ":"") . "
                   " . (($this->estuf!=''&&count($this->tpmid)>0)?" AND munt.tpmid IN ( '". (implode( "','",  $this->tpmid )) ."' ) ":"") . "
                   " . (($this->muncod!=''&&count($this->muncod)>0)?" AND MUN.muncod IN ( '". (implode( "','",  $this->muncod )) ."' ) ":"") . "
               --WHERE MUN.ESTUF IN ( '". (implode( "','", $this->estuf )) ."' )

) as cor order by cor";
		$this->dado = $db->carregar( $sql );
	}
}
/**
 * Dado - Situa��o Assessoramento Estado
 */
class DadoSituacaoAssessoramentoEstado extends DadosAbstract implements DadosInterface
{
	/**
	 * Carrega metadados da Situa��o de Assessoramento por Estado
	 * 
	 * @return void
	 * @internal carrrega $this->metaDados
	 * @todo buscar da Modelo
	 */
	public function carregaDado(){
		global $db;
		
		$sql = "
			SELECT
				s.stacor as cor,
				CASE
					WHEN s.stacod IS NULL THEN 'N�o Cadastrado'
					ELSE s.stadsc
				END as situacao,
				a.estuf as estuf
			FROM sase.assessoramentoestado a
			--INNER JOIN workflow.documento d ON d.docid = a.docid
			--INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid
			INNER JOIN sase.situacaoassessoramento s ON a.stacod = s.stacod
			WHERE 1=1
			".( (is_array($this->estuf))?" AND a.estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
		";
		$this->dado = $db->carregar( $sql );
	}
}
/**
 * Dado - Macro-Categoria
 */
class DadoMacroCategoria extends DadosAbstract implements DadosInterface
{
	/**
	 * Carrega metadados da Situação de Assessoramento Baseado na MacroCategoria
	 * 
	 * @return void
	 * @internal carrrega $this->metaDados
	 */
	public function carregaDado(){
		global $db;

		$sql = "
			SELECT 
				--m.maccor as cor, 
				s.stacor as cor,
				CASE 
					WHEN m.macdsc IS NULL THEN 'Não Cadastrado'
					ELSE m.macdsc 
				END as situacao,
				a.muncod as muncod, 
				a.estuf as estuf
			FROM sase.assessoramento a
			INNER JOIN workflow.documento d ON d.docid = a.docid
			INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid
			INNER JOIN sase.macrocategoria m ON s.maccod = m.maccod
			WHERE 1=1
			".( (is_array($this->EstadosSolicitados))?" AND a.estuf IN ('".(implode("','",$this->EstadosSolicitados))."') ":"" )."
		";
		$this->dado = $db->carregar( $sql );
	}
}
/**
 * Dado - Situação Questões Pontuais
 */
class DadoSituacaoQuestoesPontuais extends DadosAbstract implements DadosInterface
{
	/**
	 * Carrega metadados da Situação de Questões Pontuais
	 * 
	 * @return void
	 * @internal carrrega $this->metaDados
	 */
	public function carregaDado(){
		global $db;

		$sql = "
			SELECT 
				s.sqpcor as cor, 
				CASE 
					WHEN s.sqpcod IS NULL THEN 'Não Cadastrado' 
					ELSE s.sqpdsc 
				END as situacao,
				a.muncod as muncod, 
				a.estuf as estuf
			FROM sase.questoespontuaispar a
			JOIN sase.situacaoquestoespontuais s ON s.sqpcod = a.sqpcod
			WHERE 1=1
			".( (is_array($this->EstadosSolicitados))?" AND a.estuf IN ('".(implode("','",$this->EstadosSolicitados))."') ":"" )."
		";
		$this->dado = $db->carregar( $sql );
	}
}
/**
 * Dado - Mesoregião
 */
class DadoMesoregiao extends DadosAbstract implements DadosInterface
{
	/**
	 * Carrega metadados da Situação de Mesoregião
	 * 
	 * @return void
	 * @internal carrrega $this->metaDados
	 */
	public function carregaDado(){
		global $db;

		$sql = "
			SELECT 
				m.mescor as cor,
				m.mesdsc as situacao, 
				t.muncod as muncod, 
				mun.estuf as estuf
			FROM territorios.municipio mun
			JOIN sase.territorio t ON mun.muncod = t.muncod
			JOIN sase.mesoregiao m ON m.mesid = t.mesid
			WHERE 1=1
			".( (is_array($this->EstadosSolicitados))?" AND mun.estuf IN ('".(implode("','",$this->EstadosSolicitados))."') ":"" )."
			" . ( ( $this->OrgaosSolicitados )?" AND m.orgid = '" . $this->OrgaosSolicitados . "' ":"" ) . "
		";
		$this->dado = $db->carregar( $sql );
	}
}
/**
 * Dado - Estados em relação ao país - assessoramento
 */
class DadoEstadosPais extends DadosAbstract implements DadosInterface
{
	/**
	 * Carrega metadados da Situação de Mesoregião
	 * 
	 * @return void
	 * @internal carrrega $this->metaDados
	 */
	public function carregaDado(){
		global $db;

/*		$sql = "
			SELECT 
				m.maccor as cor, 
				CASE 
					WHEN s.stacod IS NULL THEN 'Não Cadastrado' 
					ELSE s.stadsc 
				END as situacao, 
				a.estuf as estuf 
			FROM sase.assessoramentoestado a 
			INNER JOIN sase.situacaoassessoramento s ON s.stacod = a.stacod 
			INNER JOIN sase.macrocategoria m ON m.maccod = s.maccod
		";*/
		$sql = "
			SELECT
				s.stacor as cor,
				CASE
					WHEN s.stacod IS NULL THEN 'Não Cadastrado'
					ELSE s.stadsc
				END as situacao,
				a.estuf as estuf
			FROM sase.assessoramentoestado a
			INNER JOIN sase.situacaoassessoramento s ON s.stacod = a.stacod
		";
		
		$this->dado = $db->carregar( $sql );
	}
}