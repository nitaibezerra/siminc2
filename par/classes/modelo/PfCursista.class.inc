<?php

class PfCursista extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.pfcursista";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pcuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'pcuid' => null,
									  	'adpid' => null,
    									'pfcid' => null,
									  	'entid' => null,
									  	'pcucpf' => null,
									  	'pcunome' => null,
										'tvpid' => null,
										'tfoid' => null,
										'pffid' => null,
										'pcuemail' => null,
										'pcudddnumtelefone' => null,
										'pcunumtelefone' => null,
                                                                                'pcuramaltelefone' => null,
    									'pcufuncao' => null,
    									'pcuvinculo' => null,
    									'pcuano' => null,
    									'pcuorgao' => null,
    									'pcucargoefetivo' => null,
   										'pcuexerciciosecretariaeducacao' => null,
    									'pcufasecurso' => null
									  );

	public function recuperarPorAdpid($adpid, $pcuid = null)
	{

		$sql = "SELECT DISTINCT
					pcu.*
				FROM par.pfcursista pcu
				WHERE pcu.adpid = {$adpid}
				AND pcu.pcuid = {$pcuid}";

		if($pcuid > 0){
			return $this->pegaLinha($sql);
		}
	}

	//verificaEntidadeExiste
	public function verificaCursistaExiste($pcucpf, $adpid, $funid, $pfcid)
	{
//		$sql = "SELECT
//					pcu.entid
//				FROM {$this->stNomeTabela} pcu
//				INNER JOIN entidade.entidade ent ON ent.entid = pcu.entid
//				INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid = {$funid}
//				WHERE pcu.entid = {$entid}
//				AND pcu.adpid = {$adpid}
//				AND pcu.pfcid = {$pfcid}";

		$sql = "SELECT
					pcu.entid
				FROM {$this->stNomeTabela} pcu
				WHERE pcu.adpid = {$adpid}
				--AND pcu.pfcid = {$pfcid}
				AND pcu.pcucpf = '{$pcucpf}'";

		return $this->pegaUm($sql);
	}

	public function verificaEntidadeCoordenadorTutorExiste($pcucpf, $adpid)
	{
		$sql = "SELECT
					pfe.entid,
					fue.funid
				FROM par.pfentidade pfe
				INNER JOIN entidade.entidade 		ent ON ent.entid = pfe.entid
				INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid IN (".FUNID_PROFESSOR_TUTOR.",".FUNID_COORDENADOR_PROFUN.")
				WHERE ent.entnumcpfcnpj = '{$pcucpf}'
				AND pfe.adpid = {$adpid}";

		return $this->pegaLinha($sql);
	}

}