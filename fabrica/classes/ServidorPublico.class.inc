<?php

/**
 * Servidor Publico
 * @author rayner
 *
 */
class ServidorPublico extends Modelo {

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "siape.tb_siape_cadastro_servidor_ativos";

	/**
	 * Chave primaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("nu_cpf");

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
        'nu_cpf' => null,
        'nu_matricula_siape' => null,
        'no_servidor' => null,
        'dt_nascimento' => null,
        'ds_atividade' => null,
        'ds_email' => null
	);
	
	protected $stCampos = "nu_cpf, nu_matricula_siape, no_servidor, dt_nascimento, ds_atividade, ds_email";
	
	
	public function setCpf($cpf){
		$this->arAtributos['nu_cpf'] = $cpf;
	}

	public function getCpf(){
		return $this->arAtributos['nu_cpf'];
	}

	public function setMatriculaSiape($matriculaSiape){
		$this->arAtributos['nu_matricula_siape'] = $matriculaSiape;
	}

	public function getMatriculaSiape(){
		return $this->arAtributos['nu_matricula_siape'];
	}
	
	public function setNome($nome){
		$this->arAtributos['no_servidor'] = $nome;
	}

	public function getNome(){
		return $this->arAtributos['no_servidor'];
	}

	public function setDataNascimento($dataNascimento){
		$this->arAtributos['dt_nascimento'] = strpos($dataNascimento, "/") ? $this->limpaMascaraData($dataNascimento) : $dataNascimento ;
	}

	public function getDataNascimento(){
		return new DateTime($this->arAtributos['dt_nascimento']);
	}

	public function setEmailServidor($email){
		$this->arAtributos['ds_email'] = $email;
	}

	public function getEmailServidor(){
		return $this->arAtributos['ds_email'];
	}

	public function recuperaServidorPublicoPeloCpf($cpf){
		$arClausulas = array("nu_cpf = '$cpf'");
    	$resultSet = parent::recuperarTodos($this->stCampos, $arClausulas);
    	return $this->montaObjeto($resultSet);
	}
	
	private function montaObjeto($resultSet){
		foreach ($resultSet as $linha){
			$s = new ServidorPublico();
			$s->setCpf($linha['nu_cpf']);
			$s->setDataNascimento($linha['dt_nascimento']);
			$s->setEmailServidor($linha['ds_email']);
			$s->setMatriculaSiape($linha['nu_matricula_siape']);
			$s->setNome($linha['no_servidor']);
		}
		return $s;
	}
	
	private function limpaMascaraData($data){
		if (strpos($data, "/")) {
			$dia = substr($data, 0, 2);
			$mes = substr($data, 3, 2);
			$ano = substr($data, 6, 4);
			return "$ano-$mes-$dia";
		} else {
			return $data;
		}
	}
}