<?php
	
include_once APPRAIZ . 'maismedicos/classes/Planotrabalho.class.inc';

class Planotrabalho_VisitaProgramada extends Planotrabalho{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "maismedicos.planotrabalho_visitaprogramada";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptvid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptvid' => null, 
									  	'ptrid' => null, 
									  	'pteid' => null, 
									  	'uniid' => null, 
									  	'ifeid' => null, 
									  	'ifcid' => null, 
									  	'arqid' => null, 
									  	'ptvdtprevisto' => null, 
									  	'ptvdtprevini' => null, 
									  	'ptvdtprevfim' => null, 
									  	'ptvobs' => null, 
									  	'ptvexecutado' => null, 
									  	'ptvdtexecutado' => null, 
									  	'ptvdtexecini' => null, 
									  	'ptvdtexecfim' => null, 
									  	'arqidexecutado' => null, 
									  	'ptvobsexecutado' => null, 
									  	'ptvdtenvioexec' => null, 
									  	'ptvstatus' => null, 
									  );
    
    public function salvaVisitaProgramadaPT($post = null, $files = null)
    {
    	
    	//if($post['ptvid'][0]>0){
    
    		if($post['ifeid_visita']){
    			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    			$file = new FilesSimec("planotrabalho_visitaprogramada", array(''), "maismedicos");
    			foreach($post['ifeid_visita'] as $k => $v){
    				
    				$arDados['ptvid'] = $post['ptvid'][$k];
    				if(empty($post['ptvid'][$k])){
    					$arDados['ptrid'] = $post['ptrid'];
    				}
	    			if(empty($post['ptvdtenvioexec'][$k])){
	    				$arDados['pteid'] = $post['pteid'];
    				}
    				//$arDados['uniid'] = $post['uniid_visita'][$k];
    				$arDados['ifeid'] = $post['ifeid_visita'][$k];
    				$arDados['ifcid'] = $post['ifcid_visita'][$k];
    				
    				if(!empty($post['ptvdtprevisto'][$k]))
    					$arDados['ptvdtprevisto'] = formata_data_sql($post['ptvdtprevisto'][$k]);
    				if(!empty($post['ptvdtprevini'][$k]))
    					$arDados['ptvdtprevini'] = formata_data_sql($post['ptvdtprevini'][$k]);
    				if(!empty($post['ptvdtprevfim'][$k]))
    					$arDados['ptvdtprevfim'] = formata_data_sql($post['ptvdtprevfim'][$k]);
    				
    				if(!empty($post['ptvdtexecutado'][$k]))
    					$arDados['ptvdtexecutado'] = formata_data_sql($post['ptvdtexecutado'][$k]);
    				if(!empty($post['ptvdtexecini'][$k]))
    					$arDados['ptvdtexecini'] = formata_data_sql($post['ptvdtexecini'][$k]);
    				if(!empty($post['ptvdtexecfim'][$k]))
    					$arDados['ptvdtexecfim'] = formata_data_sql($post['ptvdtexecfim'][$k]);
    				
    				$arDados['ptvobs'] = $post['ptvobs'][$k];
    				
    				if(!empty($post['ptvobsexecutado'][$k]))
    					$arDados['ptvobsexecutado'] = $post['ptvobsexecutado'][$k];
    				
    				if($post['enviar_execucao']=='true' && empty($post['ptvdtenvioexec'][$k]) && !empty($post['ptvdtexecini'][$k]))
    					$arDados['ptvdtenvioexec'] = date('Y-m-d');
    				
    				
    				//echo '<pre>'; var_dump($arDados);die;
    				if($files['arquivo_visita']['name'][$k]){
    
    					$_FILES['arquivo']['name'] = $files['arquivo_visita']['name'][$k];
    					$_FILES['arquivo']['type'] = $files['arquivo_visita']['type'][$k];
    					$_FILES['arquivo']['tmp_name'] = $files['arquivo_visita']['tmp_name'][$k];
    					$_FILES['arquivo']['error'] = $files['arquivo_visita']['error'][$k];
    					$_FILES['arquivo']['size'] = $files['arquivo_visita']['size'][$k];
    
    					$file->setUpload(null, "arquivo", false);
    					$arDados['arqid'] = $file->getIdArquivo();
    				}
    				
    				if($files['arquivo_visita_exec']['name'][$k]){
    
    					$_FILES['arquivo']['name'] = $files['arquivo_visita_exec']['name'][$k];
    					$_FILES['arquivo']['type'] = $files['arquivo_visita_exec']['type'][$k];
    					$_FILES['arquivo']['tmp_name'] = $files['arquivo_visita_exec']['tmp_name'][$k];
    					$_FILES['arquivo']['error'] = $files['arquivo_visita_exec']['error'][$k];
    					$_FILES['arquivo']['size'] = $files['arquivo_visita_exec']['size'][$k];
    
    					$file->setUpload(null, "arquivo", false);
    					$arDados['arqidexecutado'] = $file->getIdArquivo();
    				}
    				//ver($arDados, $post, d);
    				if(empty($post['ptvdtenvioexec'][$k])){
    				$this->popularDadosObjeto($arDados);
    				$ptvid = $this->salvar();
    				$this->commit();
    				$this->clearDados();
    				if($ptvid>0) $arIds[] = $ptvid;
    				}
    				unset($arDados);
    			}
    			
    			if($arIds){
    				$this->executar("update maismedicos.planotrabalho_visitaprogramada set ptvstatus = 'I' where ptrid = {$post['ptrid']} and ptvid not in (".implode(',',$arIds).")");$this->commit();
    			}
    		}
    	//}
    }
    
    
}