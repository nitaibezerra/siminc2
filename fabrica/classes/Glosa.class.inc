<?php
/**
 * Entidade criada com função de recusa total ou parcial da
 * contagem de ponto de função de determinada Ordem de Serviço.
 * Toda Ordem de Servico que possuir Glosa deverá ter sua contagem
 * de pontos de função subtraída do valor em PF da Glosa
 * @author rayner
 *
 */
class Glosa extends Modelo {
/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "fabrica.glosa";

	/**
	 * Chave primaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array("glosaid");

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
        'glosaid' => null,
        'glosaqtdepf' => null,
        'glosajustificativa' => null,
        'glosadatainclusao' => null,
        'glosacpfusuarioresponsavel' => null
	);
	
	public function getId(){
		return $this->arAtributos['glosaid'];
	}

	public function setId($id){
		$this->arAtributos['glosaid'] = $id;
	}

	public function getValorEmPf(){
		return $this->arAtributos['glosaqtdepf'];
	}

	public function  setValorEmPf($valorEmPf){
		$this->arAtributos['glosaqtdepf'] = $valorEmPf;
	}
	
	public function getValorEmPfComMascara(){
		return str_replace('.', ',', $this->arAtributos['glosaqtdepf']);
	}

	public function  setValorEmPfComMascara($valorEmPf){
		$this->arAtributos['glosaqtdepf'] = str_replace(',', '.', $valorEmPf);
	}

	public function getJustificativa(){
		return $this->arAtributos['glosajustificativa'];
	}

	public function setJustificativa($justificativa){
		$this->arAtributos['glosajustificativa'] = $justificativa;
	}

	public function getDataInclusao(){
		return new DateTime($this->arAtributos['glosadatainclusao']);
	}

	public function setDataInclusao($dataInclusao){
		$this->arAtributos['glosadatainclusao'] = $dataInclusao;
	}

	public function getUsuarioResponsavel(){
		return $this->arAtributos['glosacpfusuarioresponsavel'];
	}

	public function setUsuarioResponsavel($usuarioResponsavel){
		$this->arAtributos['glosacpfusuarioresponsavel'] = $usuarioResponsavel;
	}
	
	public function salvar($idOrdemServico){
		if ($this->getId()=='' || $this->getId()==null){
			$novoIdGlosa = parent::salvar($arAtributos);
			$this->commit();
			$this->atribuirGlosaAOrdemDeServico($novoIdGlosa, $idOrdemServico);
			return $novoIdGlosa;
		} else {
			$qtde = $this->getValorEmPf();
			$justificativa = $this->getJustificativa();
			$data = $this->getDataInclusao()->format("Y-m-d");
			$cpf = $this->getUsuarioResponsavel();
			$id = $this->getId();
			
			$sql = "UPDATE fabrica.glosa SET glosaqtdepf = $qtde, 
				glosajustificativa = '$justificativa', glosadatainclusao = '$data', 
				glosacpfusuarioresponsavel = '$cpf' WHERE glosaid = $id";
			return (int)(parent::executar($sql) && parent::commit());
			
		}
	}
	
	private function atribuirGlosaAOrdemDeServico($novoIdGlosa, $idOrdemServico){
		$sql = "UPDATE fabrica.ordemservico SET glosaid = '$novoIdGlosa' WHERE odsid = $idOrdemServico";
		parent::executar($sql);
		parent::commit();
	}
	
	public function removeGlosa($idGlosa, $idOrdemServico){
		$sql = "UPDATE fabrica.ordemservico SET glosaid = null WHERE odsid = $idOrdemServico";
		parent::executar($sql);
		$sql = "DELETE FROM fabrica.glosa where glosaid = $idGlosa";
		parent::executar($sql);
		parent::commit();
	}
	
	public function recupereGlosaPeloId($idGlosa){
		$sql = "SELECT * FROM fabrica.glosa where glosaid = $idGlosa";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto($linha){
		$glosa = new Glosa();
		$glosa->setId($linha['glosaid']);
		$glosa->setDataInclusao(new DateTime($linha['glosadatainclusao']));
		$glosa->setJustificativa($linha['glosajustificativa']);
		$glosa->setUsuarioResponsavel($linha['glosacpfusuarioresponsavel']);
		$glosa->setValorEmPf($linha['glosaqtdepf']);
		return $glosa;
	}

}