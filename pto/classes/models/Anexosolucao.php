<?php

include_once APPRAIZ . "includes/classes/file.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Model_Anexosolucao extends Abstract_Model
{

	protected $_schema = 'pto';
	protected $_name = 'anexosolucao';
	public $entity = array();

	public function __construct($commit = true)
	{
		parent::__construct($commit);
		$this->perfilUsuario = new Model_PerfilUsuario();

		$this->entity['anxid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
		$this->entity['arqid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Baixar Arquivo');
		$this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label' => 'solid');
		$this->entity['anxdtinclusao'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '', 'label' => 'Data Inclusão');
		$this->entity['anxstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
		$this->entity['anxdesc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '255', 'contraint' => '', 'label' => 'Descrição');
	}

	public function getDados($solid = null)
	{
		$solid = (is_int($solid) ? $solid : $_SESSION['solid']);
		$sql = "
             SELECT anexosol.anxid, anexosol.arqid, anexosol.anxdesc, anexosol.anxdtinclusao
                FROM pto.anexosolucao anexosol
                WHERE anexosol.anxstatus = 'A'
                AND solid = {$solid}
                ORDER BY anexosol.anxdtinclusao ASC
         ";

		$dados = $this->_db->carregar($sql);

		if (is_array($dados)) {
			foreach ($dados as $key => $valor) {
				if ($valor['anxdtinclusao']) {
					$dados[$key]['anxdtinclusao'] = date('d/m/Y', strtotime($valor['anxdtinclusao']));
				}
				if($valor['arqid']){
					$dados[$key]['arqid'] = "<a href='pto.php?modulo=principal/download&acao=A&arqid={$valor['arqid']}' class='download_grid'><i class='glyphicon glyphicon-download-alt'></i></a>";
				}
			}
		}
		return $dados;
	}

	public function getListing($solid = null)
	{
		$listing = new Listing(false);
		$listing->setPerPage(999999);
		$listing->setIdTable('table_boletim');
		$listing->setClassTable('table table-striped table-bordered sorted_table');

		$listing->setHead(array($this->getAttributeLabel('arqid'), $this->getAttributeLabel('anxdesc'), $this->getAttributeLabel('anxdtinclusao')));

		if (!$this->perfilUsuario->possuiAcessoConsulta()) {
			$listing->setActions(array('delete' => 'excluir_boletim'));
		}

		$data = $this->getDados($solid);
		return $listing->listing($data);
	}

	public function salvarBoletim()
	{
		$solid = ($_SESSION['solid'] ? $_SESSION['solid'] : $this->getPost('solid'));

		$this->populateEntity($_POST);
		$this->setAttributeValue('solid', $solid);
		$this->setAttributeValue('anxstatus', 'A');
		$this->setAttributeValue('anxdtinclusao', date('Y-m-d'));

		if ($this->gravarArquivo()) {
			$id = $this->save();
			if ($id == false) {
				throw new Exception('Erro ao inserir Boletim.');
			} else {
				return $id;
			}
		}else{
			$this->error[] = array("name" => 'file_boletim', "msg" => ('É necessário selecionar um anexo'));
			throw new Exception('Erro ao inserir Anexo Boletim.');
		}


	}

	public function gravarArquivo()
	{
		if (is_uploaded_file($_FILES['file_boletim']['tmp_name'])) {
			$this->setArqId();
		}
		$arqid = $this->getAttributeValue('arqid');

		if (empty($arqid)) {
			return false;
		}
		return true;
	}

	public function setArqId($descricao = "PTO - Anexo Boletim", $campo = "file_boletim")
	{
		$file = new FilesSimec($this->_name, null, $this->_schema);
		$file->setUpload($descricao, $campo, false, false);
		$arqid = (int)$file->getIdArquivo();
		$this->setAttributeValue('arqid', $arqid);
	}

	public function inativar($id)
	{
		$this->populateEntity(array('anxid' => $id));
		$this->setAttributeValue('anxstatus', 'I');

		$this->setDecode(false);
		$this->treatEntityToUser();

		$id = $this->update();
		if ($id == false) {
			throw new Exception('Erro ao excluir o anexo.');
		} else {
			return $id;
		}
	}

	public function getArquivo($idArquivo)
	{
		$file = new FilesSimec();
		$file->getDownloadArquivo($idArquivo);
	}
}
