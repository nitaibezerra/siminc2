<?php
class Questionario extends Modelo {
	
	/**
	 * Nome da tabela especificada
	 * 
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela =  "consultapne.questionario";
	
	/**
	 * Chave primaria.
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"queid" 
	);
	
	/**
	 * Atributos
	 * 
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'queid' => null,
			'parid' => null,
			'quedtcriacao' => null,
			'quedtfinalizacao' => null,
			'quesituacao' => null
	);
	
	public function recuperarPorParticipante($parid) {
		$sql = " SELECT * FROM $this->stNomeTabela WHERE parid = '{$parid}'; ";
	
		$arResultado = $this->pegaLinha( $sql );
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
	
		return $this;
	}
	
	public function finalizar() 
	{
		try {

            $erro = $this->verificarQuestionario();

            if($erro){
               return $erro;
            }

			$this->carregarPorId($_SESSION['queid_pne']);
			$this->quesituacao = 'F';
			$this->quedtfinalizacao = date('Y-m-d H:i:s');
			$this->alterar();
			$this->commit();

            return false;
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
	}

    public function verificarQuestionario()
    {
        $sql = "select parid from consultapne.participante
                where parid = {$_SESSION['parid_pne']}
                and (
                           (coalesce(parcpf, '') = '')
                        or (coalesce(estuf, '') = '')
                        or (coalesce(muncod, '') = '')
                        or (coalesce(paremail, '') = '')
                        or (coalesce(escid, 0) = 0)
                        or (coalesce(atuid, 0) = 0)
                        /*
                        or ((parrepresentacao!=3) and (coalesce(tpoid, 0) = 0))
                        or ((parrepresentacao!=3) and (coalesce(parcnpj, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parreprazaosocial, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parrepnomefantasia, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parrepuf, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parrepmuncod, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parrepnome, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parrepcpf, '') = ''))
                        or ((parrepresentacao!=3) and (coalesce(parrepemail, '') = ''))
                        */
                )
                union
                select 1
                from consultapne.avaliacao
                where queid = {$_SESSION['queid_pne']}
                having count(*) = 0
                ";
        $campoVazio = $this->pegaUm( $sql );

        return $campoVazio ? 'Favor preencher todos os campos obrigatórios (informações gerais e pelo menos um item de qualquer indicador)' : false;
    }
}