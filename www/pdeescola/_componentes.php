<?php
class formulario{
	private $db;
    public function __construct($dados = array()){
    	global $db;
    	
    	$this->db = $db;  	
    	$this->pagAtual = $_REQUEST['modulo'];
    	
    	if (is_array($dados)){
	     	foreach ($dados as $k => $val){
		    	 $this->$k = $val;
	     	}
    	}
    	// Tratamento Exercicio base de comparacao - exibir dados do ano de exercicio ate ano vigente.
    	if( $_SESSION['exercicio'] != $_SESSION['exercicio_atual']) {
			echo "<script language=\javascript\" type=\"text/javascript\">
						alert('Nenhum dado para este ano Exercício Base de Comparação.');
						window.history.go(-1);
		 		  </script>";
							exit;	
			}
    	if($this->instrumento == 1) {
    		$nome = $db->carregar("SELECT 
    								trim(epfnomearquivo) as nome
    							  FROM 
    							  	pdeescola.estruturaperfilfuncionamento
    							  WHERE
    							  	epfnomearquivo is not null AND
    							  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA."
    							  ORDER BY
    							  	epfordem");
    		
    		$nomeArquivo = Array();
    		for($i=0; $i<count($nome); $i++) {
    			$nomeArquivo[$i] = $nome[$i]["nome"];
    		}
    		
    		$this->instrumento1 = $nomeArquivo;
    		/*$this->instrumento1 = array("p1",
										  "p7",
										  "p8",
										  "p9_1",
										  "p10_1",
										  "p10_2",
    									  "p10_3",
    									  "p10_4",
    									  "p10_5",
    									  "p10_6",
    									  "p10_7",
    									  "p10_8a",
    									  "p10_8b",
    									  "p10_8c",
										  "p11_1",
    									  "p11_2",
										  "p12_1",
										  "p13",
										  "p14",
										  "p15",
										  "p16",
										  "p17",
										  "p18",
										  "p19",
										  "p20",
										  "p21",
										  "p22",
										  "p23",
										  "p24",
										  "p25",
    									  "p26",
    									  "p27");*/
    		
	    }elseif ($this->instrumento == 2){
	    	$this->instrumento2 = array();
	    	
			 /*
			 * Carrega perguntas instrumento 2
			 */
			$sql = "SELECT
					 DISTINCT
					 p2.aceid,
					 p2.acecodigo
					FROM 
					 pdeescola.analisecriterioeficacia p
					 INNER JOIN pdeescola.analisecriterioeficacia p2 ON (p2.aceidpai = p.aceid AND p2.aceidpai IS NOT NULL AND p2.aceseq IS NOT NULL)
					WHERE
					 p2.aceano = ".ANO_EXERCICIO_PDE_ESCOLA." 
					ORDER BY
					 p2.acecodigo";
			$perg = (array) $this->db->carregar($sql);
			foreach ($perg as $perg){
				
				$perg1 = array_map(trim,$perg);
				$url   = "cadastro_instrumento2&acao=A&aceid={$perg1['aceid']}";
	    		array_push($this->instrumento2,$url);
			}
			array_push($this->instrumento2,"cadastro_criticidade");
			array_push($this->instrumento2,"cadastro_prioridade");
			array_push($this->instrumento2,"total_pontos");
	    }
        elseif( $this->instrumento == 'sintese_autoavaliacao' )
        {
         
            $this->instrumentosintese_autoavaliacao = array("problemas_criterios",
										  "problemas_causas_acoes",
										  "previsao_recursos",
										  "objetivos_estrategias_metas");
            
        }elseif ($this->instrumento == 3){
	    	$this->instrumento3 = array();
	    	
			 /*
			 * Carrega perguntas monitoramento
			 */
			$sql = "SELECT
					 DISTINCT
					 qap2.qapid,
					 qap2.qapcodigo,
					 qap2.qapidpai
					FROM 
					 pdeescola.questaoavaliacaoplano qap
					 INNER JOIN pdeescola.questaoavaliacaoplano qap2 ON (qap2.qapidpai = qap.qapid AND qap2.qapidpai IS NOT NULL AND qap2.qapidpai <> 1 AND qap2.qapidpai <> 28 
					 )
					WHERE
					 qap2.qapano = ".ANO_EXERCICIO_PDE_ESCOLA."
					ORDER BY
					 qap2.qapid";
			 
			$perg = (array) $this->db->carregar($sql);
			foreach ($perg as $perg){
				
				$perg1 = array_map(trim,$perg);
				$url   = "cadastroMonitoramento&acao=A&qapid={$perg1['qapid']}";
	    		array_push($this->instrumento3,$url);
			}
	    }
    }
    
    
    
    function montarbuttons ($validaForm = null, $dir = 'estrutura_avaliacao'){
    	
    	$verificaAutoavaliacao = verificaPreenchimentoAutoavaliacao();
    	
    	$pagAtual 	  = end(explode('/',$this->pagAtual));	
    	$instrumento  = $this->instrumento;
    	$instrumento1 = $this->{'instrumento'.$instrumento};
    	
    	if(!$verificaAutoavaliacao){
    		$validaForm = str_replace(";","",$validaForm);
    	}else{    	
    		$validaForm = null;
    	}
    	
		if ($instrumento && $pagAtual){

			$arrAtual = (int) array_search($pagAtual,$instrumento1);
			$arrAtual = (int) $arrAtual ? $arrAtual : array_search( end(explode('/',$_SERVER['REQUEST_URI'])),$instrumento1); 
			
			$pro = (int) $arrAtual + 1;
			$ant = (int) $arrAtual - 1;    

			$perfis = arrayAcessoPerfil();
			
			$docid  = pegarDocid( $_SESSION['entid'] );
			$estado = pegarEstadoAtual( $docid );
 			
			if( $estado == ESTADO_EM_ELABORACAO || $estado == ESTADO_EM_CORRECAO ){
				if (in_array( PDEESC_PERFIL_SUPER_USUARIO ,arrayPerfil() ) || in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis) || in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) {
					$buttons = "<input type='button' class='botao' name='salvarAnt' id='salvarAnt' value='Salvar Anterior' ".($ant < 0 ? 'disabled' : '')." title='Salvar e pergunta anterior' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 3; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." />
								<input type='button' class='botao' name='salvar' id='salvar' value='Salvar' title='Salvar' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 5; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." />
								<input type='button' class='botao' name='salvarPro' id='salvarPro' value='Salvar Próximo' ".($pro >= count($instrumento1) ? 'disabled' : '')." title='Salvar e próxima pergunta' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 4; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." /><BR>";					
				}
			}elseif(($estado == ENVIADO_PARA_PAGAMENTO) || ($estado == VALIDACAO_PELO_MEC_WF) || ($estado == DEVOLVIDO_PARA_ESCOLA_PC_WF) || ($estado == AVALIACAO_COMITE_ME_WF) || ($estado == DEVOLVIDO_PARA_COMITE_WF) || ($estado == AVALIACAO_MEC_PARCERIA_COMPLEMENTAR_WF) || ($estado == ENVIADO_PARA_PAGAMENTO_WF ) && $dir == 'propostaMonitoramento'){
				if (in_array( PDEESC_PERFIL_SUPER_USUARIO ,arrayPerfil() ) || in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis) || in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) {
					$buttons = "<input type='button' class='botao' name='salvarAnt' id='salvarAnt' value='Salvar Anterior' ".($ant < 0 ? 'disabled' : '')." title='Salvar e pergunta anterior' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 3; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." />
								<input type='button' class='botao' name='salvar' id='salvar' value='Salvar' title='Salvar' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 5; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." />
								<input type='button' class='botao' name='salvarPro' id='salvarPro' value='Salvar Próximo' ".($pro >= count($instrumento1) ? 'disabled' : '')." title='Salvar e próxima pergunta' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 4; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." /><BR>";					
				}
			}elseif( in_array( PDEESC_PERFIL_SUPER_USUARIO ,arrayPerfil() ) ){
				$buttons = "<input type='button' class='botao' name='salvarAnt' id='salvarAnt' value='Salvar Anterior' ".($ant < 0 ? 'disabled' : '')." title='Salvar e pergunta anterior' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 3; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." />
								<input type='button' class='botao' name='salvar' id='salvar' value='Salvar' title='Salvar' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 5; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." />
								<input type='button' class='botao' name='salvarPro' id='salvarPro' value='Salvar Próximo' ".($pro >= count($instrumento1) ? 'disabled' : '')." title='Salvar e próxima pergunta' onclick='javascript:abilitar(0); document.getElementById(\"controlador\").value = 4; if(!".$validaForm."){abilitar(1);}' ".($validaForm == null ? 'disabled="disabled"' : '')." /><BR>";					
			}
			
			echo "<tr bgcolor=\"#C0C0C0\">
						<td colspan=\"10\" align='center' valign='top'>
							<input type='hidden' name='controlador' id='controlador' value=''>
							<div style=\"display:inline;\">
								<div style=\"float:left; display:inline;\">
									<input type='button' class='botao' name='anterior' id='anterior' ".($ant < 0 ? 'disabled' : '')." value='Anterior' title='Pergunta anterior' onclick='javascript:document.getElementById(\"controlador\").value = 1; this.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.submit();'/>
								</div>
								<div style=\"float:right; display:inline;\">
									<input type='button' class='botao' name='proximo' id='proximo' value='Próximo' ".($pro >= count($instrumento1) ? 'disabled' : '')." title='Pergunta próxima' onclick='javascript:document.getElementById(\"controlador\").value = 2; this.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.submit();'/>
								</div>								
								<div style=\"position:relative; display:inline;\">
								".$buttons."
								<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='location.href=\"?modulo=principal/".$dir."&acao=A\";' title='Voltar para Árvore' />
								</div>
							</div>						
						</td>			
				   </tr>
				   <script>
				   function abilitar(param){
				   		var d 		  = document;
				   		var salvarAnt = d.getElementById('salvarAnt');
				   		var salvar 	  = d.getElementById('salvar');
				   		var salvarPro = d.getElementById('salvarPro');
				   //		var anterior  = d.getElementById('anterior');
				   //		var proximo   = d.getElementById('proximo');
				   //		var voltar	  = d.getElementById('btFechar');				   		
				   		
				   		//alert(param);
				   		if (param == 0){
				   			salvarAnt.disabled = true;
				   			salvar.disabled    = true;
				   			salvarPro.disabled = true;
				   		//	anterior.disabled  = true;
				   		//	proximo.disabled   = true;
				   		//	voltar.disabled    = true;
				   		}else{
				   			salvarAnt.disabled = false;
				   			salvar.disabled    = false;
				   			salvarPro.disabled = false;
				   		//	anterior.disabled  = false;
				   		//	proximo.disabled   = false;
				   		//	voltar.disabled    = false;
				   		}
				   }
				   </script>";
		}
			
	}
	
	function direcPag($funcao=null, $dados = array()){
		$instrumento  = $this->instrumento;
		
		$instrumentoArr = $this->{'instrumento'.$instrumento};
		$pagAtual 	    = end(explode('/',$this->pagAtual));	
		$arrAtual 	    = array_search($pagAtual,$instrumentoArr);
		$arrAtual		= $arrAtual ? $arrAtual : array_search( end(explode('/',$_SERVER['REQUEST_URI'])),$instrumentoArr); 

		/****************************************
		 * $_POST['controlador']
		 * 1 => pagina anterior
		 * 2 => proxima pagina
		 * 3 => Salva pagina anterior
		 * 4 => Salva proxima pagina
		 * 5 => Salva e permanece na mesma página
		 ****************************************/	

		if ($_POST['controlador'] == 1){
			$newPag	  = $instrumentoArr[($arrAtual-1)];
 
		}elseif ($_POST['controlador'] == 2){
			$newPag	  = $instrumentoArr[($arrAtual+1)];

		}elseif ($_POST['controlador'] == 3){
			$newPag	  = $instrumentoArr[($arrAtual-1)];
			$chamada = $this->tratarChamada($funcao, $dados);
			call_user_func_array( $chamada['funcao'], $chamada['parametros'] );
			$text = 'alert(\'Operação realizada com sucesso!\')';
		}elseif ($_POST['controlador'] == 4){
			$newPag	  = $instrumentoArr[($arrAtual+1)];
			$chamada = $this->tratarChamada($funcao, $dados);
			call_user_func_array( $chamada['funcao'], $chamada['parametros'] );
			$text = 'alert(\'Operação realizada com sucesso!\')';
		}elseif ($_POST['controlador'] == 5){
			$newPag  = $instrumentoArr[$arrAtual];
			$chamada = $this->tratarChamada($funcao, $dados);
			call_user_func_array( $chamada['funcao'], $chamada['parametros'] );
			$text = 'alert(\'Operação realizada com sucesso!\');';		
		}else{
			return;
		}
		//echo'?modulo=principal/'.(is_numeric($instrumento) ? 'instrumento'.$instrumento : $instrumento).'/'.(strpos($newPag,"&acao=") ? $newPag : $newPag.'&acao=A');
		//die();
		
		die('<script>
				'.$text.'
				//alert(\'?modulo=principal/'.(is_numeric($instrumento) ? 'instrumento'.$instrumento : $instrumento).'/'.(strpos($newPag,"&acao=") ? $newPag : $newPag.'&acao=A').'\');
				location.href = \'?modulo=principal/'.(is_numeric($instrumento) ? 'instrumento'.$instrumento : $instrumento).'/'.(strpos($newPag,"&acao=") ? $newPag : $newPag.'&acao=A').'\';
			 </script>');
	}

	function tratarChamada( $chamada, array $dados )
	{
		
		// verifica se formato básico da condição
		$posAbre = strpos( $chamada, "(" );
		$posFecha = strrpos( $chamada, ")" );
		if ( $posAbre === false || $posFecha === false )
		{
			return array(
				"funcao" => "",
				"parametros" => array()
			);
		}
		
		// captura a funcao
		$funcao = trim( substr( $chamada, 0, $posAbre ) );
		
		// verifica se função é "chamável" 
		if ( !is_callable( $funcao ) )
		{
			return array(
				"funcao" => "",
				"parametros" => array()
			);
		}
		
		// captura parâmetros
		$parametrosCru = substr( $chamada, $posAbre + 1, $posFecha - $posAbre - 1 );
		$parametrosCru = explode( ",", trim( $parametrosCru ) );
		$parametrosCru = array_map( "trim", $parametrosCru );
		$parametros = array();

		foreach ( $parametrosCru as $item )
		{
			if ( array_key_exists( $item, $dados ) )
			{
				array_push( $parametros, $dados[$item] );
			}
		}
		
		return array(
			"funcao" => $funcao,
			"parametros" => $parametros
		);
	}
		
}

?>