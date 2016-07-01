<?php
/**
 * Description of QuestoesCumprimentoObjetoAnexos
 *
 * @author lindalbertofilho
 */
class QuestoesCumprimentoObjetoAnexos extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questoes_cumprimentoobjeto_anexos";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qcoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qcoid' => null,
        'qrpid' => null,
        'perid' => null,
        'arqid' => null,
      );

	public function verifica( $qrpid, $perid )
    {
		$sql = "SELECT qcoid FROM {$this->stNomeTabela} WHERE qrpid = {$qrpid} AND perid = {$perid}";
		$qcoid = $this->pegaUm( $sql );
		if ($qcoid) {
			$sql = "DELETE FROM questionario.resposta WHERE qrpid = ".$qrpid." AND perid = {$perid};";
			$sql .= "DELETE FROM {$this->stNomeTabela} WHERE qcoid = ".$qcoid."; ";
			$this->executar($sql);
		}
		return true;
	}

	public function pegaResposta( $qrpid, $perid )
    {
		$sql = "
            SELECT
                *
            FROM {$this->stNomeTabela} a
            INNER JOIN public.arquivo ar on ar.arqid = a.arqid
            WHERE a.qrpid = {$qrpid}
                AND a.perid = {$perid}";
		return $this->pegaLinha( $sql );
	}

	public function deletaAnexo($arqid)
    {
		$sql = "SELECT qrpid, perid FROM {$this->stNomeTabela} WHERE arqid = ".$arqid;
		$dados = $this->pegaLinha($sql);
        $file = new FilesSimec('questoes_cumprimentoobjeto_anexos', $this->arAtributos, 'obras2');
        $file->setPulaTableEschema();
        $file->setRemoveUpload($arqid);

        $sql = "DELETE FROM questionario.resposta r WHERE r.qrpid = ".$dados['qrpid']." AND r.perid = ".$dados['perid'].";";
        $this->executar($sql);
        $this->commit();
		return true;
	}
}