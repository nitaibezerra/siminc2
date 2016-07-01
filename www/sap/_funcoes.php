<?php 

	/**
	 * Recupera o titulo da tela das tabelas de apoio 
	 * @param int $abacod - codigo da aba
	 * @param string $url - url da tela
	 * @return string
	 */
	function consultarTituloTela($abacod, $url){
	    
		global $db;
	
	    $sql = "select m.mnudsc
	              	from seguranca.menu m
	            where
	            	m.mnulink = '$url'";
	
	    return $db->pegaUm($sql);
	}
	
	/**
	 * Exibe mensagem de alerta no sistema 
	 * @param string $mensagem - Mensagem para ser exibida
	 */
	function alerta($mensagem){
		
		if(!empty($mensagem))
			echo "<script type='text/javascript'>alert('{$mensagem}')</script>";
		
	}
	
	
	
	/**
     * Método responsável por redirecionar para página solicitada e exibir uma mensagem passada como parâmetro
     *
     * @name direcionar
     * @author 
     * @access public
     * @return mensagem do sucesso ou fracasso
     */
	function direcionar($url, $msg=null){
		if($msg){
			echo "<script>
	                alert('$msg');
	                window.location='$url';
	              </script>";
		} else{
			echo "<script>
	                window.location='$url';
	              </script>";
		}
		exit;
	}
	
	
	/**
     * Método responsável por executar scripts da tela pai partindo da popup
     *
     * @name executarScriptPai
     * @author Cézar Cirqueira
     * @access public
     * @return 
     */
	function executarScriptPai($funcao){
		echo "	<script>
					executarScriptPai('$funcao');
				</script>";
	}
	
	/**
     * Método responsável por fechar popups
     *
     * @name fecharPopup
     * @author Cézar Cirqueira
     * @access public
     * @return 
     */
	function fecharPopup(){
		echo "	<script>
					self.close();
				</script>";
	}
	
	/**
     * Formata o valor numeric para ser inserido no banco 
     * @name fecharPopup
     * @author Silas Matheus
     * @access public
     * @return float
     */
	function formata_valor_sql($valor){
		
		$valor = str_replace('.', '', $valor);
		$valor = str_replace(',', '.', $valor);
		
		return $valor;
		
	}
	
?>