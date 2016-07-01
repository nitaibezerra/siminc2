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
	public $orgid;
    public $subid;
    public $pneano;
    public $tpdid;

	protected $metaDados = array();
	protected $Dado;

	/**
	 * @param string $classeDado
	 */
	public function montaDado( $classeDado ){
		$this->Dado = new $classeDado();
		
		if( !empty($this->estuf) ) $this->Dado->estuf = $this->estuf;
		if( !empty($this->muncod) ) $this->Dado->muncod = $this->muncod;
		if( !empty($this->orgid) ) $this->Dado->orgid = $this->orgid;
        if( !empty($this->subid) ) $this->Dado->subid = $this->subid;
        if( !empty($this->pneano) ) $this->Dado->pneano = $this->pneano;
        if( !empty($this->tpdid) ) $this->Dado->tpdid = $this->tpdid;

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

		$poligonosDecodificados = json_decode( $Poligonos->poligono );

        if( !is_array($poligonosDecodificados) ) return false;

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
        //return json_encode( $poligonosDecodificados);
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
    public $subid = array();
    public $pneano;
    public $tpdid;

	public $dado;
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

        if ($this->estuf === 'undefined') $this->estuf = '';

		$sql = "
			SELECT 
				s.stacor as cor, 
				CASE 
					WHEN s.stacod IS NULL THEN 'Não Cadastrado' 
					ELSE s.stadsc 
				END as situacao, 
				a.muncod as muncod, 
				a.estuf as estuf 
			FROM sase.assessoramento a 
			INNER JOIN workflow.documento d ON d.docid = a.docid
			INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid 
			WHERE 1=1 
			".( (is_array($this->estuf) && count($this->estuf) > 0)?" AND a.estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
		";

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
			JOIN sase.orgaoestado oes ON m.oesid = oes.oesid
			WHERE 1=1
			".( (is_array( $this->estuf))?" AND mun.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
			" . ( ( $this->orgid )?" AND oes.orgid = '" . $this->orgid . "' ":"" ) . "
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
/**
 * Dado - Plano de carreira dos professores
 */

class DadoPlanoCarreiraMunicipio extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Mesoregião
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
            select
                case
                    when pcp.docid is null then '#FFFFFF'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spccor
                            when {$this->tpdid} = 238 then spc.spccor
                          end
                end as cor,
                case
                    when pcp.docid is null then 'Sem Informa��o'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spmdsc
                            when {$this->tpdid} = 238 then spc.spcdsc
                          end
                end as situacao,
                pcp.muncod as muncod,
                pcp.estuf as estuf
            from sase.planocarreiraprofessor pcp
            left join workflow.documento doc on doc.docid = pcp.docid
            left join workflow.documento doc2 on doc2.docid = pcp.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
			WHERE 1=1
			".( (is_array( $this->estuf))?" AND pcp.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
		";

        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Planod de carreira por estado
 */
class DadoPlanoCarreiraEstado extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Mesoregião
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        $sql = "
            select
                case
                    when pce.docid is null then '#FFFFFF'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spccor
                            when {$this->tpdid} = 238 then spc.spccor
                          end
                end as cor,
                case
                    when pce.docid is null then 'Sem Informa��o'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spmdsc
                            when {$this->tpdid} = 238 then spc.spcdsc
                          end
                end as situacao,
                pce.estuf as estuf
            from sase.planocarreiraprofessorestado pce
            left join workflow.documento doc on doc.docid = pce.docid
            left join workflow.documento doc2 on doc2.docid = pce.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
			WHERE 1=1
			".( (is_array( $this->estuf))?" AND pce.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
		";

        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Plano de carreira por estado - externo
 */
class DadoPlanocarreiraEstadoExterno extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situa��o de Estado
     *
     * @return void
     * @internal carrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        $sql = "
            select
                case
                    when pce.docid is null then '#FFFFFF'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spccor
                            when {$this->tpdid} = 238 then spc.spccor
                          end
                end as cor,
                case
                    when pce.docid is null then 'Sem Informa��o'
                    else CASE
                            when {$this->tpdid} = 240 then spm.spmdsc
                            when {$this->tpdid} = 238 then spc.spcdsc
                          end
                end as situacao,
                pce.estuf as estuf
            from sase.planocarreiraprofessorestado pce
            left join workflow.documento doc on doc.docid = pce.docid
            left join workflow.documento doc2 on doc2.docid = pce.docid2
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid";

        $this->dado = $db->carregar( $sql );
    }
}

/**
 * Dado - Acompanhamento PNE - Munic�pio
 */
class DadoAcompanhoPneMunicipio extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Mesoregião
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado(){
        global $db;
        if ($this->estuf === 'undefined') $this->estuf = '';
        if (is_array( $this->estuf) && $this->estuf[0] == "") $this->estuf = '';
        $sql = "
            with temp_pne as (select pnevalormeta, pnevalor, muncod, pneano from sase.pne pne where pne.muncod is not null and subid = {$this->subid} and pneano = '{$this->pneano}'),
                 temp_cor as (select legid, legfxa1, legfxa2 from sase.legendaindicadores where subid = {$this->subid})
            select
                muncod,
                estuf,
                leg.legcor as cor,
                case
                    when leg.legid is null then 'Sem Informa��o'
                    else 'Faixa de '||leg.legfxa1||' � '||leg.legfxa2
                end as situacao
            from (select
                    tab.muncod,
                    tab.mundescricao,
                    tab.pnevalormeta,
                    tab.pnevalor,
                    tab.estuf,
                    (
                        select legid from temp_cor where tab.pnevalormeta between legfxa1 and legfxa2
                    ) as legid,
                    (
                        select legid from temp_cor where tab.pnevalor between legfxa1 and legfxa2
                    ) as legid2
                from (select
                        mun.muncod,
                        mun.mundescricao,
                        mun.estuf,
                        pne.pnevalormeta,
                        pne.pnevalor
                    from territorios.municipio mun
                    inner join temp_pne pne on mun.muncod = pne.muncod
                    ".( (is_array( $this->estuf) )?" WHERE mun.estuf IN ('".(implode("','", $this->estuf))."') ":"" )."
                    order by mundescricao) as tab) as tab2
            left join sase.legendaindicadores leg on tab2.legid2 = leg.legid";

        $this->dado = $db->carregar( $sql );
    }
}