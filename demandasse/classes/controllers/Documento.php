<?php
/**
 * Controle responsavel pelos documentos.
 *
 * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
 * @since  13/10/2014
 *
 * @name       Documento
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Documento extends Abstract_Controller
{
    protected $_model;

    public function __construct()
    {
        parent::__construct();
        $this->_model = new Model_Demanda();
    }

    public function indexAction()
    {
        $modelTipoDocumento = new Model_Tipodocumento();
        $modelProcedencia = new Model_Procedencia();

        $this->view->tipoDocumento = $modelTipoDocumento->getAllByValues(array('tpdstatus' => 'A'));
        $this->view->procedencias = $modelProcedencia->getAllByValues(array('prcstatus' => 'A'));

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction()
    {
        $modelTipoDocumento = new Model_Tipodocumento();
        $modelProcedencia = new Model_Procedencia();

        $this->view->tipoDocumento = $modelTipoDocumento->getAllByValues(array('tpdstatus' => 'A'));
        $this->view->procedencias = $modelProcedencia->getAllByValues(array('prcstatus' => 'A'));

        $id = $this->getPost('id');
        $this->_model->populateEntity(array( 'dmdid' => $id));

        $this->dateConvert($this->_model->entity['dmddtentdocumento']['value']);
        $this->dateConvert($this->_model->entity['dmddtemidocumento']['value']);
        $this->dateConvert($this->_model->entity['dmdprazoemdata']['value']);

        $this->view->entity = $this->_model->entity;



        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction()
    {
        //mantendo filtros na sessao para recuperar acao pesquisar após submeter workflow
        $_SESSION['dados_filtro_documento'] = $_POST    = simec_utf8_decode_recursive($_POST);
        
        $dmdid = $this->getPost('dmdid');
        if($dmdid){
            $where = " AND dmdid = {$dmdid}";
        } else {
            $where = $this->_model->searchWhere($_POST);

            if($where){
                $where = str_replace('tpdid' , 'tpd.tpdid' , $where);
                $where = " AND {$where}";
            }
        }
       if ($_POST['ordenacao']) {
            if ($_POST['ordenacao'] == 'dmdprazoemdata') {
                $order = " ORDER BY {$_POST['ordenacao']}::date {$_POST['tipoOrdenacao']}";
            } else {
                $order = " ORDER BY {$_POST['ordenacao']} {$_POST['tipoOrdenacao']}";
            }
        }
        if ($_POST['esdid'] && $_POST['esdid'] != '0'){
        	$where .= " AND ed.esdid = " . $_POST['esdid'];
        }
        
        if ($_POST['chkAtrasado']){
			$where .= " AND dmdprazoemdata < now() AND ed.esdid NOT IN (" . ESDID_ARQUIVADO . ")";
        }


        if ($_POST['chkreiteracao']){
            $wReiteracao = " AND dmdreiteracao = 't'";
        }
        $where .= $wReiteracao;

        $listing = new Listing();

        $listing->enableCount(true);
        $listing->setActions(array('edit' => 'editar' , 'delete' => 'excluir' , 'file' => 'file' , 'envelope' => 'enviaremail'));
        $listing->setHead(array('N&ordm; do Documento' , 'Tipo de documento' , 'Data do documento' , 'Prazo em dias', 'Prazo em Data' ,
                                'Interessado' , 'Assunto' , 'Destino' , 'Situa&ccedil;&atilde;o' , 'Refer&ecirc;ncia' , 'Sidoc', 'Reiteração'));
        $data = "WITH demandaalteracao AS
  (SELECT dma.dmdid,
          dma.dmaassunto,
          dma.dmaprazoemdias,
          dma.dmaprazoemdata
   FROM demandasse.demandaalteracao dma
   WHERE dma.dmaid =
       (SELECT MAX(dma2.dmaid)
        FROM demandasse.demandaalteracao dma2
        WHERE dma.dmdid = dma2.dmdid))
SELECT dmdid,
       dmdnumdocumento AS numero,
       tpddsc,
       TO_CHAR(dmddtentdocumento, 'DD/MM/YYYY') AS dmddtentdocumento,
       COALESCE(dma.dmaprazoemdias, dmd.dmdprazoemdias) AS dmdprazoemdias,
       (CASE
            WHEN COALESCE(dma.dmaprazoemdata, dmd.dmdprazoemdata) IS NOT NULL THEN 
            (CASE WHEN to_char(COALESCE(dma.dmaprazoemdata, dmd.dmdprazoemdata)::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD') 
                  THEN '<font color=\"#FBB917\" title=\"Documento com vencimento hoje!\">' || to_char(COALESCE(dma.dmaprazoemdata, dmd.dmdprazoemdata), 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" value=\"green\"> </font>' 
                  ELSE 
                    (CASE WHEN COALESCE(dma.dmaprazoemdata, dmd.dmdprazoemdata) < CURRENT_DATE 
                          THEN '<font color=\"red\" title=\"Documento em atraso!\">' || to_char(COALESCE(dma.dmaprazoemdata, dmd.dmdprazoemdata), 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" value=\"red\"> </font>' 
                          ELSE '<font color=\"green\" title=\"Documento em dia!\">' || to_char(COALESCE(dma.dmaprazoemdata, dmd.dmdprazoemdata), 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" value=\"yellow\"> </font>' 
                    END) 
            END) 
       END) AS dmdprazoemdata,
       prc_orig.prcsigla || ' - ' || prc_orig.prcdsc AS prcdsc_orig,
       COALESCE(dma.dmaassunto, dmd.dmdassunto) AS dmdassunto,
       prc_dest.prcdsc AS prcdsc_dest,
       ed.esddsc,
       dmdreferencia,
       dmdnumsidoc,
       (CASE
            WHEN dmdreiteracao = 't' THEN 'Sim'
            ELSE 'Não'
        END) AS dmdreiteracao
FROM demandasse.demanda dmd
LEFT JOIN demandasse.tipodocumento tpd ON (tpd.tpdid = dmd.tpdid)
LEFT JOIN demandasse.procedencia prc_orig ON (prc_orig.prcid = dmd.prcid_orig)
LEFT JOIN demandasse.procedencia prc_dest ON (prc_dest.prcid = dmd.prcid_dest)
LEFT JOIN workflow.documento d ON (d.docid = dmd.docid)
LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
LEFT JOIN demandaalteracao dma USING(dmdid)
WHERE dmdstatus = 'A'
{$where} {$order}";
      
        $this->view->exibirTitulo = true;
        $this->view->data = $data;
        $this->view->listing = $listing;

        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function limparAction()
    {
        unset($_SESSION['dados_filtro_documento']);
        unset($_POST);
        $this->view->data = (array( 'action' => 'listar'));
        $return = array('status' => true);
    	echo simec_json_encode($return);
//        $this->_forward('documento/listar');
    }
    public function listarPdfAction(){
        global $db;
    	$dmdid = $this->getPost('dmdid');

    	if($dmdid){
    		$where = " AND dmdid = {$dmdid}";
    	} else {
    		$where = $this->_model->searchWhere($_POST);
    	
    		if($where){
    			$where = str_replace('tpdid' , 'tpd.tpdid' , $where);
    			$where = " AND {$where}";
    		}
    	}
          if ($_POST['ordenacao']) {
            if ($_POST['ordenacao'] == 'dmdprazoemdata') {
                $order = " ORDER BY {$_POST['ordenacao']}::date {$_POST['tipoOrdenacao']}";
            } else {
                $order = " ORDER BY {$_POST['ordenacao']} {$_POST['tipoOrdenacao']}";
            }
        }
    	if ($_POST['esdid'] && $_POST['esdid'] != '0'){
    		$where .= " AND ed.esdid = " . $_POST['esdid'];
    	}
    	
    	if ($_POST['chkAtrasado']){
    		$where .= " AND dmdprazoemdata < now() AND ed.esdid NOT IN (" . ESDID_ARQUIVADO . ")";
    	}
    	$listing = new Listing();
               $listing->setEnablePagination(false);
               
               $queryQnt = "SELECT count(dmdnumdocumento) as numero
    						 				FROM demandasse.demanda dmd
    						 				LEFT JOIN demandasse.tipodocumento tpd ON (tpd.tpdid = dmd.tpdid)
    						 				LEFT JOIN demandasse.procedencia prc_orig ON (prc_orig.prcid = dmd.prcid_orig)
    						 				LEFT JOIN demandasse.procedencia prc_dest ON (prc_dest.prcid = dmd.prcid_dest)
    						 				LEFT JOIN workflow.documento d ON ( d.docid = dmd.docid)
    						 				LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
    						 				WHERE dmdstatus = 'A'
    						 				{$where}";
               $qntRegistros = $db->pegaUm($queryQnt);
               $listing->setPerPage($qntRegistros);
    	
    	        $listing->setHead(array('N° do Documento' , 'Tipo de documento' , 'Data do documento' , 'Prazo em dias', 'Prazo em Data' ,
                                'Interessado' , 'Assunto' , 'Destino' , 'Situação' , 'Referência' , 'Sidoc'));
    	$data = "SELECT dmdnumdocumento as numero, tpddsc , TO_CHAR(dmddtentdocumento , 'DD/MM/YYYY') AS dmddtentdocumento , dmdprazoemdias,
        				--TO_CHAR(dmdprazoemdata , 'DD/MM/YYYY') AS dmdprazoemdata,
        				(CASE WHEN dmdprazoemdata is not null THEN
							(CASE WHEN to_char(dmdprazoemdata::date,'YYYY-MM-DD') = to_char(CURRENT_DATE::date,'YYYY-MM-DD')
					 			THEN '<font color=\"#FBB917\" title=\"Documento com vencimento hoje!\">' || to_char(dmdprazoemdata, 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" value=\"green\"> </font>' 
					 			ELSE 
					 				--(CASE WHEN ed.esdid IN (" . ESDID_ARQUIVADO . ")
        								--THEN '<font title=\"Documento com vencimento hoje!\">' || to_char(dmdprazoemdata, 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" > </font>'
        								--ELSE 
											(CASE WHEN dmdprazoemdata < CURRENT_DATE
									 			THEN '<font color=\"red\" title=\"Documento em atraso!\">' || to_char(dmdprazoemdata, 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" value=\"red\"> </font>' 
									 			ELSE '<font color=\"green\" title=\"Documento em dia!\">' || to_char(dmdprazoemdata, 'DD/MM/YYYY') || ' <input type=\"hidden\" name=\"dtprazo['||dmdid||']\" id=\"dtprazo['||dmdid||']\" value=\"yellow\"> </font>' 
									 		END)						 		
        							END)
					 		--END)						 		
				 		END) AS dmdprazoemdata,
    						 				prc_orig.prcsigla || ' - ' || prc_orig.prcdsc as prcdsc_orig , dmdassunto , prc_dest.prcdsc as prcdsc_dest , ed.esddsc, dmdreferencia , dmdnumsidoc
    						 				FROM demandasse.demanda dmd
    						 				LEFT JOIN demandasse.tipodocumento tpd ON (tpd.tpdid = dmd.tpdid)
    						 				LEFT JOIN demandasse.procedencia prc_orig ON (prc_orig.prcid = dmd.prcid_orig)
    						 				LEFT JOIN demandasse.procedencia prc_dest ON (prc_dest.prcid = dmd.prcid_dest)
    						 				LEFT JOIN workflow.documento d ON ( d.docid = dmd.docid)
    						 				LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
    						 				WHERE dmdstatus = 'A'
    						 				{$where} {$order}";
    						 				 
    						 				$this->view->exibirTitulo = true;
    						 				$this->view->data = $data;
    						 				$this->view->listing = $listing;
                                                                         
    	//$content = $this->render(__CLASS__, __FUNCTION__);
    	ob_start();
        $color_th = 'th.mescla{background-color:#C0C0C0; border: 1px solid #CCCCCC;}';
        $listing->setIdTable('table_documento');
		$listing->setClassTable('listagem');
		$listing->listing_tabulado($data,'',$color_odd = 'style="background-color: #CCCCCC"', $color_table_border = '', $color_th);
        //		$this->render(__CLASS__, __FUNCTION__);
		$content = ob_get_contents();
		ob_end_clean();
        $content = '<style> body{font-size: 08px;}font{color:red;}</style><p align="CENTER">MINISTÉRIO DA EDUCAÇÃO<br>SECRETARIA EXECUTIVA<br>DEMANDAS SE</p><br>'.$content;
        //   die($content);
    	//print $content;
    	html2Pdf($content);
    }

    public function salvarAction(){
        global $db;
        
        $dmdid = trim( $_POST['dmdid'] );
        $tpdid = trim( $_POST['tpdid'] );
        $dmdnumdocumento = trim($_POST['dmdnumdocumento']);
        
//        $documentoC = false;


        #- COMO NÃO PASSADO NO POST O dmdid, SIGNIFICA QUE É UMA NOVA INSERÇÃO.
        #REGRA: Não pode ser inserido um novo doCumento que: 
        # - NÚMERO E TIPO DO DOCUMENTO JÁ EXISTA. (NA BASE)
        #VERIFICAR: SE NA BASE TEM REGISTRO COM O dmdnumdocumento e tpdid

        if( !empty($dmdnumdocumento) && !empty($tpdid) ){
            $sqlDoc = "
                SELECT  tpdid AS tipo_doc,
                        dmdnumdocumento AS num_doc
                FROM demandasse.demanda dmd 
                WHERE dmdnumdocumento = '{$dmdnumdocumento}' AND tpdid = '{$tpdid}'
            ";
            //$documentoC = $db->pegaUm($sqlDoc);
            $dados = $db->pegaLinha($sqlDoc);
        }
        
        #CASO EXISTA "VALORES" NOS CAMPOS dmdnumdocumento, tpdid E ESTAJA SENDO PASSADO NO POST O dmdid  SE CARACTERIZA UPDATE E PODE SER FEITO, CASO CONTRARIO NÃO.
        if( ( $dados['num_doc'] == $dmdnumdocumento && $dados['tipo_doc'] == $tpdid ) && $dmdid == '' ){
        
        //if ($documentoC) {
            $return = array('status' => false, 'msg' => utf8_encode('Os dados não foram salvos, documento já cadastrado para este tipo!'), 'result' => $this->_model->error);
        } else {
            if (!$_POST['dmdid']) {
                $_POST['usucpfinclusao'] = "{$_SESSION['usucpf']}";
                $_POST['dmddtinclusao'] = 'now()';
                $_POST['dmdstatus'] = 'A';
            } else {
                $_POST['dmddtalteracao'] = 'now()';
            }

            $this->_model->populateEntity($_POST);

            if (empty($this->_model->entity['docid']['value'])) {
                $doc_id = wf_cadastrarDocumento(WF_TPDID_DEMANDASSE_DEMANDA, "Fluxo de documentos do Demandas SE");
                $this->_model->entity['docid']['value'] = $doc_id;
            }

            $id = $this->_model->save();
            if ($this->_model->error) {
                $return = array('status' => false, 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $this->_model->error);
            } else {
                $return = array('status' => true, 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id, 'docid'=>$doc_id);
            }
        }
        echo simec_json_encode($return);
    }
    
    //function criada para atualizar somente div do workflow 
    public function atualizarworkflowAction()
    {
         $this->view->docid = $_POST['docid'];
         $this->render(__CLASS__, __FUNCTION__);
    }
    public function deletarAction()
    {
        global $db;
        
        $id = $this->getPost('id');
        $sql = "DELETE FROM demandasse.demandaalteracao WHERE dmdid = '{$id}'";
        $db->executar($sql);
        $this->_model->setDecode(false);
        $this->_model->populateEntity(array( 'dmdid' => $id));
        $result = $this->_model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }
        
        echo simec_json_encode($return);
    }
    
        
	public function formularioMensagemAction()
    {
        $modelTipoDocumento = new Model_Tipodocumento();
        $modelProcedencia = new Model_Procedencia();

        $this->view->tipoDocumento = $modelTipoDocumento->getAllByValues(array('tpdstatus' => 'A'));
        $this->view->procedencias = $modelProcedencia->getAllByValues(array('prcstatus' => 'A'));
        
        $id = $this->getPost('id');
        $this->_model->populateEntity(array( 'dmdid' => $id));

        $this->view->entity = $this->_model->entity;

        $this->render(__CLASS__, __FUNCTION__);
    }
    
	public function listarMensagemAction()
    {
        $dmdid = $this->getPost('id');

        $listing = new Listing();

        $listing->enableCount(true);
        //$listing->setActions(array('edit' => 'editarArquivo' , 'delete' => 'excluirArquivo' , 'download-alt' => 'downloadArquivo'));
        $listing->setHead(array('Data do Envio','Mensagem'));
        $data = "SELECT TO_CHAR(ecpdtenvio , 'DD/MM/YYYY') as ecpdtenvio , ecpcorpoemail
                    FROM demandasse.emailcobrancaprazo dma
                    WHERE dmdid = {$dmdid}";

        $this->view->exibirTitulo = true;
        $this->view->data = $data;
        $this->view->listing = $listing;

        $this->render(__CLASS__, __FUNCTION__);
    }
    
    
	public function salvarMensagemAction()
    {
    	/*
        if(!$_POST['dmaid']){
            $_POST['usucpfinclusao'] = "{$_SESSION['usucpf']}";
            $_POST['dmadtinclusao'] = 'now()';
            $_POST['dmastatus'] = 'A';
        } else {
            $_POST['dmddtalteracao'] = 'now()';
            $_POST['usucpfalteracao'] = "{$_SESSION['usucpf']}";
        }

        $this->_model->populateEntity($_POST);

        if(empty($this->_model->entity['arqid']['value'])){
        }

        $id = $this->_model->save();

        if($this->_model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $this->_model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }
		*/
    	
    	global $db;
    	
    	$id = $_POST['dmdid'];
    	
    	$sql = "INSERT INTO demandasse.emailcobrancaprazo(dmdid, usucpf, ecpemailde, ecpemailpara, 
    														ecpemailcc, ecpassunto, ecpcorpoemail, ecpdtenvio)
    			VALUES ($id, 
    					'".$_SESSION['usucpf']."', 
    					'".$_SESSION['email_sistema']. "', 
    					'".$_POST['ecpemailpara']."',  
    					NULL, 
    					'".$_POST['ecpassunto']."',  
            			'".$_POST['ecpcorpoemail']."', 
            			now())";
    	
    	$db->executar($sql);
    	$db->commit();
    	
    	$return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
    	
        echo simec_json_encode($return);
    }
    
    public function liberar_alteracao($docid){
        $estado = wf_pegarEstadoAtual($docid);
        if ($estado['esdid'] == ESD_DEMANDA_EM_ATENDIMENTO || $estado['esdid'] == ESD_DEMANDA_EM_DILIGENCIA) {
            $array_habil['readonly'] = 'readonly = "readonly"';
            $array_habil['disabled'] = ' disabled="true"';
        }
        return $array_habil;
    }
    
    public function buscar_historico_documento($id) {

        global $db;
        $sql = "SELECT dmaid, docid, dmaassunto, dmaprazoemdias, to_char(dmaprazoemdata, 'dd/mm/YYYY') as dmaprazoemdata
                FROM demandasse.demandaalteracao alt inner join demandasse.demanda dem on dem.dmdid = alt.dmdid where 
                alt.dmdid = {$id} order by dmaid desc limit 1";
        $registro = $db->pegaLinha($sql);
        return $registro;
    }

    public function alterarAction() {
        global $db;
        $assunto = utf8_decode($_POST['dmdassunto']);
        if(!$_POST['dmdprazoemdias']){
            $_POST['dmdprazoemdias'] = 'NULL';
        }
        
        if(!$_POST['dmdprazoemdata']){
            $_POST['dmdprazoemdata'] = 'NULL';
        }else{
           // $_POST['dmdprazoemdata'] = formata_data_sql($_POST['dmdprazoemdata']); 
             $_POST['dmdprazoemdata'] = "to_date('{$_POST['dmdprazoemdata']}','YYYY-mm-dd')"; 
        }
        
        $sql = "INSERT INTO demandasse.demandaalteracao(
            dmdid, dmaassunto, dmaprazoemdias, dmaprazoemdata, usucpfalteracao, 
            dmddtalteracao)
    VALUES ( {$_POST['dmdid']}, '{$assunto}', {$_POST['dmdprazoemdias']}, {$_POST['dmdprazoemdata']}, 
            '{$_SESSION['usucpf']}', NOW()) returning dmaid";

        $id = $db->pegaUm($sql);
        $db->commit();

        $return = array('status' => true, 'msg' => utf8_encode('As alterações foram salvas!'), 'result' => 'id = ' . $id, 'docid'=>$_POST['docid']);

        echo simec_json_encode($return);
    }

}