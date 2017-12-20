<?php
/*
 * Classe para consulta da base da Receita Federal para construção de formulários de acordo com a tabela do banco
 * @author Juliano Meinen de Souza
 * @since 17/01/2012
 * @exemple:
 * 	include  APPRAIZ."includes/classes/consultaReceitaFederal.class.inc";
	$teste = new ConsultaReceitaFederal("F","numero do cpf");
	$teste->recuperaDadosReceita();
 */
class ConsultaReceitaFederal extends cls_banco{
	
	public $cnpjcpf = null;
	private $tipo = null;
	private $arrErros = array();
	private $arrErrosProgramador = array();
	private $schema = null;
	private $table = null;
	private $endereco_schema = null;
	private $endereco_table = null;
	public $formulario = array();
	public $arrCampos = array();
	private $arrValores = array();
	private $arrEndereco = array();  
	
	public function __construct( $tipo , $cnpjcpf = null , $schema = null , $tabela = null)
	{
		if($tipo != "J" && $tipo != "F"){
			$this->retornaErroProgramador("Tipo inválido! Informe 'F' para Pessoa Física e 'J' para Pessoa Jurídica.");
		}else{
			$this->tipo = $tipo;
		}
		
		if($cnpjcpf){
			if(strlen($cnpjcpf) > 14){
				if($this->validaCNPJ($cnpjcpf)){
					$this->cnpj = $cnpjcpf;
				}
			}else{
				if($this->validaCPF($cnpjcpf)){
					
					$this->cpf = $cnpjcpf;
				}
			}
		}
		
		if($schema){
			$this->schema = $schema;
		}
		if($tabela){
			$this->table = $tabela;
		}
		
		parent::__construct();
	}

	public function CPF($cpf)
	{
		if($this->tipo == "J"){
			$this->retornaErroProgramador("O Tipo defino no contrutor é Pessoa Física ('F') .");
		}
		
		if($this->validaCPF($cpf)){
			$this->cpf = $cpf;
		}else{
			dbg($this->arrErros);
		}
	}
	
	public function CNPJ($cnpj)
	{
		if($this->tipo == "F"){
			$this->retornaErroProgramador("O Tipo defino no contrutor é Pessoa Jurídica ('J') .");
		}
		if($this->validaCNPJ($cnpj)){
			$this->cnpj = $cnpj;
		}else{
			dbg($this->arrErros);
		}
	}
	
	public function atributosFormulario($arrAttb)
	{
		$this->formulario = $arrAttb;
	}
	
	public function camposFormulario($arrCampos)
	{
		$this->arrCampos = $arrCampos;
	}
	
	public function schema($schema)
	{
		$this->schema = $schema;
	}
	
	public function table($table)
	{
		$this->table = $table;
	}
	
	public function enderecoSchema($endereco_schema)
	{
		$this->endereco_schema = $endereco_schema;
	}
	
	public function enderecoTable($endereco_table)
	{
		$this->endereco_table = $endereco_table;
	}
	
	public function  recuperaDadosReceita()
	{
		if($this->cnpj && $this->tipo == "J"){
			require_once APPRAIZ . 'www/includes/webservice/pj.php';
            ver(1231, d);
			$objPessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
			$pj = str_replace(array('/', '.', '-'), '', $this->cnpj);
			$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
			$obj = (object) simplexml_load_string($xml);
			if (!$obj->PESSOA) {
				$this->retornaErroUsuario("CNPJ inexistente na base da Receita Federal.");
			}
			return $xml;
		}elseif($this->cpf && $this->tipo == "F"){
			require_once APPRAIZ . 'www/includes/webservice/cpf.php';
			$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
			$cpf = str_replace(array('/', '.', '-'), '', $this->cpf);
			$xml = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($cpf);
			$obj = (object) simplexml_load_string($xml);
			if (!$obj->PESSOA) {
				$this->retornaErroUsuario("CPF inexistente na base da Receita Federal.");
			}
			return $xml;
		}else{
			$this->retornaErroProgramador("Informe o ".($this->tipo == "F" ? "CPF" : "CNPJ")."." );
		}
	}
	
	public function exibirFormulario()
	{
		$attbCampo = $this->recuperaAtributosColuna();
		?>
		<style>
			.msg_formulario{font-weight:bold;color:blue}
		</style>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script>
			jQuery.noConflict();
			
			function RFverificaCNPJ()
			{
				var cnpj = jQuery("[name='cnpj']").val();
				
				if(!cnpj){
					RFlimparCampos();
					return false;
				}
				
				var schema = '<?php echo $this->schema ?>';
				var table = '<?php echo $this->table ?>';
								
				//Verifica CNPJ válido
				jQuery.ajax({
					   type: "POST",
					   url: "../geral/consultaDadosReceitaFederal.php",
					   data: "requisicao=ajaxValidaCNPJ&cnpj=" + cnpj + "&tipo=<?php echo $this->tipo ?>",
					   success: function(msg){
					   		if(msg){
					   			alert(msg);
					   		}else{
						   		if(schema && table){
						   			RFcarregaDadosLocal();
						   		}else{
					   				RFcarregaDadosCNPJ();
					   			}
					   		}
					   }
					 });
			}
			
			function RFlimparCampos()
			{
				jQuery("[name='<?php echo $this->formulario['name'] ? $this->formulario['name'] : "formulario_rf" ?>'] :text").val("");
			}
			
			function RFcarregaDadosCNPJ()
			{
				var cnpj = jQuery("[name='cnpj']").val();
				jQuery.ajax({
					   type: "POST",
					   url: "../geral/consultaDadosReceitaFederal.php",
					   data: "requisicao=ajaxCarregaDadosCNPJ&cnpj=" + cnpj + "&tipo=<?php echo $this->tipo ?>",
					   dataType: "xml",
					   success: function(xml){
					   		if(xml){
						   		 jQuery(xml).find('PESSOA').each(function () {
			                        <?php if( count($this->arrCampos) ): ?>
										<?php foreach($this->arrCampos as $campo):?>
											<?php if($campo['receita']): ?>
												jQuery("[name='" + campo_<?php echo $campo['receita'] ?> + "']").val(jQuery(this).find('<?php echo $campo['receita'] ?>').text());
											<?php endif; ?>
										<?php endforeach; ?>
									<?php endif; ?>
			                        jQuery(this).find('ENDERECOS').each(function () {
				                       jQuery(this).find('ENDERECO').each(function () {
				                        	var cep = jQuery(this).find('nu_cep').text();
			                        		jQuery("[name='endcep']").val(mascaraglobal("##.###-###",cep));
			                        		jQuery("[name='endlog']").val(jQuery(this).find('ds_tipo_logradouro').text() + " " + jQuery(this).find('ds_logradouro').text());
			                        		jQuery("[name='endcom']").val(jQuery(this).find('ds_logradouro_comp').text());
			                        		jQuery("[name='endnum']").val(jQuery(this).find('ds_numero').text());
			                        		jQuery("[name='endbai']").val(jQuery(this).find('ds_bairro').text());
			                        		jQuery("[name='estuf']").val(jQuery(this).find('sg_uf').text());
			                        		var muncod = jQuery(this).find('co_cidade').text();
			                        		muncod = muncod.replace("00000000","");
			                        		jQuery("[name='muncod']").val(muncod);
			                        		RFajaxRecuperaMunicipio(muncod);
				                    	}); 
				                    });
			                    });
			                 }else{
			                 	alert('CNPJ não encontrado.');
			                 }
					   }
					 });
			}
			
			function RFcarregaDadosLocal()
			{
				var cnpj = jQuery("[name='cnpj']").val();
				jQuery.ajax({
					   type: "POST",
					   url: "../geral/consultaDadosReceitaFederal.php",
					   data: "requisicao=ajaxCarregaDadosLocal&cnpj=" + cnpj + "&tipo=<?php echo $this->tipo ?>&schema=<?php echo $this->schema?>&table=<?php echo $this->table?>&end_schema=<?php echo $this->endereco_schema?>&end_table=<?php echo $this->endereco_table?>",
					   dataType: "xml",
					   success: function(xml){
					   		if(jQuery(xml).find('SEMDADOS').length <= 0){
						   		 jQuery(xml).find('DADOS').each(function () {
			                        <?php if( count($attbCampo) ): ?>
										<?php foreach($attbCampo as $campo):?>
											if(jQuery("[name='<?php echo $campo['nome'] ?>']")){
												jQuery("[name='<?php echo $campo['nome'] ?>']").val(jQuery(this).find('<?php echo $campo['nome'] ?>').text());
											}
										<?php endforeach; ?>
									<?php endif; ?>
			                       jQuery(this).find('ENDERECO').each(function () {
			                        	var cep = jQuery(this).find('endcep').text();
		                        		jQuery("[name='endcep']").val(mascaraglobal("##.###-###",cep));
		                        		jQuery("[name='endlog']").val(jQuery(this).find('endlog').text());
		                        		jQuery("[name='endcom']").val(jQuery(this).find('endcom').text());
		                        		jQuery("[name='endnum']").val(jQuery(this).find('endnum').text());
		                        		jQuery("[name='endbai']").val(jQuery(this).find('endbai').text());
		                        		jQuery("[name='estuf']").val(jQuery(this).find('estuf').text());
		                        		jQuery("[name='muncod']").val(jQuery(this).find('muncod').text());
		                        		jQuery("[name='mundescricao']").val(jQuery(this).find('mundescricao').text());
			                    	}); 
			                    });
			                 }else{
			                 	RFcarregaDadosCNPJ();
			                 }
					   }
					 });
			}
			
			function RFverificaCEP()
			{
				var cep = jQuery("[name='endcep']").val();
			}
			
			function RFajaxRecuperaMunicipio(muncod)
			{
				jQuery.ajax({
					   type: "POST",
					   url: "../geral/consultaDadosReceitaFederal.php",
					   data: "requisicao=ajaxRecuperaMunicipio&muncod=" + muncod + "&tipo=<?php echo $this->tipo ?>",
					   success: function(msg){
					   		jQuery("[name='mundescricao']").val(msg);
					   }
					 });
			}
			
			function RFvalidacao(obj,tipo)
			{
				switch(tipo){
					case "email":
						var er = /^[a-zA-Z0-9][a-zA-Z0-9\._-]+@([a-zA-Z0-9\._-]+\.)[a-zA-Z-0-9]{2}/;
						var email = obj.value;
						if(email){
							if(er.exec(email)){
								jQuery("#msg_campo_" + obj.name).html("");
							}else{
								jQuery("#msg_campo_" + obj.name).html("* Email inválido.");
							}
						}else{
							jQuery("#msg_campo_" + obj.name).html("");
						}
					break;
				}
			}
			
			function retornoFalso(valor)
			{
			
			}
			
			function esconde(id)
			{
				jQuery("#" + id).fadeOut('slow');
			}
			
			jQuery(function() {
			
				if(jQuery('#tr_msg_formulario')){
				 	window.setTimeout( "esconde('tr_msg_formulario')" , '5000' );
				 }
				 				 
				if( jQuery("[name='cnpj']").val() ){
					jQuery("[name='cnpj']").val( mascaraglobal("##.###.###/####-##",jQuery("[name='cnpj']").val()) )
				}
				if( jQuery("[name='cpf']").val() ){
					jQuery("[name='cpf']").val( mascaraglobal("###.###.###-##",jQuery("[name='cpf']").val()) )
				}
				if( jQuery("[name='endcep']").val() ){
					jQuery("[name='endcep']").val( mascaraglobal("##.###-###",jQuery("[name='endcep']").val()) )
				}
				
				jQuery('[name=btn_salvar]').click(function() {
					var erro = 0;
					jQuery("[class~=obrigatorio]").each(function() { 
						if(!this.value || this.value == "Selecione..."){
							erro = 1;
							alert('Favor preencher todos os campos obrigatórios!');
							this.focus();
							return false;
						}
					});
					
					jQuery("[class~=msg_validacao]").each(function() { 
						var html = jQuery(this).html();
						if(html){
							erro = 1;
							alert(html.replace("*",""));
							return false;
						}
					});
															
					if(erro == 0){
						jQuery("[name='requisicao']").val("RFsalvar");
						jQuery("[name='<?php echo $this->formulario['name'] ? $this->formulario['name'] : "formulario_rf" ?>']").submit();
					}
				});
			});
			
		</script>
		<form name="<?php echo $this->formulario['name'] ? $this->formulario['name'] : "formulario_rf" ?>" id="<?php echo $this->formulario['id'] ? $this->formulario['id'] : "formulario_rf" ?>"  method="post" action="<?php $this->formulario['action']?>" enctype="multipart/form-data" >
			<input type="hidden" name="schema" value="<?php echo $this->schema ?>" />
			<input type="hidden" name="table" value="<?php echo $this->table?>" />
			<input type="hidden" name="requisicao" value="" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<?php if($_SESSION['RF_receita_federal']['msg_form']): ?>
					<tr id="tr_msg_formulario" class="center SubtituloTabela" >
						<td class="msg_formulario" colspan="2">
							<?php echo $_SESSION['RF_receita_federal']['msg_form'] ?>
							<?php unset($_SESSION['RF_receita_federal']['msg_form']) ?>
						</td>
					</tr>
				<?php endif; ?>
				<?php if($this->formulario['titulo']): ?>
					<tr class="center SubtituloTabela" >
						<td class="bold" colspan="2">
							<?php echo $this->formulario['titulo']  ?>
						</td>
					</tr>
				<?php endif; ?>
				<tr>
					<td class="SubtituloDireita" width="25%">
						<?php echo $this->tipo == "J" ? "CNPJ:" : "CPF:" ?>
					</td>
					<td>
						<?php $entnumcpfcnpj = $this->arrValores[0]['entnumcpfcnpj']; ?>
						<?php $habilitado = $entnumcpfcnpj ? "N" : "s"; ?>
						<?php if($this->tipo == "J"): ?>
							<?php $onChange = $entnumcpfcnpj ? "" : "onchange='RFverificaCNPJ()'"; ?>
							<?php echo campo_texto("cnpj","S",$habilitado,"CNPJ",22,18,"##.###.###/####-##","","right","",""," $onChange ","",$entnumcpfcnpj) ?>
						<?php else: ?>
							<?php $onChange = $entnumcpfcnpj ? "" : "onchange='RFverificaCPF()'"; ?>
							<?php echo campo_texto("cpf","S",$habilitado,"CPF",20,15,"###.###.###-##","","right","",""," $onChange ","",$entnumcpfcnpj) ?>
						<?php endif; ?>
					</td>
				</tr>
				<?php if( count($this->arrCampos) ): ?>
					<?php foreach($this->arrCampos as $campo):?>
						<?php if($campo['funcao'] != "titulo" && $campo['funcao'] != "monta_html" && $campo['funcao'] != "monta_endereco" && $campo['funcao'] != "hidden"): ?>
							<tr>
								<td class="SubtituloDireita" >
									<?php echo $campo['descricao']?>:
								</td>
								<td>
									<?php 
										switch($campo['funcao']){
											
											case "campo_texto":
												$obrig = $campo['obrigatorio'] ? "S" : "N";
												$habil = $campo['habilitado'] ? "S" : "N";
												$size  = $campo['tamanho'] ? $campo['tamanho'] : $attbCampo[$campo['nome']]['tamanho'];
												$masc  = $campo['mascara'] ? $campo['mascara'] : "";
												$max = $attbCampo[$campo['nome']]['tamanho'];
												$value = $this->arrValores[0][$campo['nome']];
												$complemento = "";
												$complemento = $campo['validacao'] ? " onchange=\"RFvalidacao(this,'{$campo['validacao']}')\" " : "";
												echo campo_texto($campo['nome'],$obrig,$habil,$campo['descricao'],$size,$max,$masc,"null", $align, $txtdica,"", $complemento,"", $value);
											break;
											
											case "monta_combo":
												$obrig = $campo['obrigatorio'] ? "S" : "N";
												$habil = $campo['habilitado'] ? "S" : "N";
												$size  = "200";
												$acao = false;
												$acao = $campo['validacao'] ? "RFvalidacao(this,'{$campo['validacao']}');retornoFalso " : "";
												$sql = "select
															{$attbCampo[$campo['nome']]['fk']['coluna']} as codigo,
															{$campo['campoCombo']} as descricao
														from
															{$attbCampo[$campo['nome']]['fk']['schema']}.{$attbCampo[$campo['nome']]['fk']['tabela']}
														where 1=1 ".($campo['where'] ? (is_array($campo['where']) ? " and ".implode(" and ",$campo['where']) : " and ".$campo['where'] ) : "")."
														order by
															descricao";
												$value = $this->arrValores[0][$campo['nome']];
												$this->monta_combo($campo['nome'],$sql,$habil,"Selecione...",$acao,$opc,$txtdica,$size,$obrig, $id, $return , $value, $title);
											break;
											
											default:
												
											break;
										
										}
									?>
									<span class="msg_validacao red" id="msg_campo_<?php echo $campo['nome'] ?>" style="padding-left:10px"  ></span>
									<script> var campo_<?php echo $campo['receita'] ?> = '<?php echo $campo['nome'] ?>' </script>
								</td>
							</tr>
						<?php else: ?>
							<?php if($campo['funcao'] == "monta_endereco"): ?>
								<?php $this->montaEndereco($campo['campo'],$campo['schema'],$campo['tabela']); ?>
							<?php endif; ?>
							<?php if($campo['funcao'] == "monta_html"): ?>
								<?php echo $campo['html'] ?>
							<?php endif; ?>
							<?php if($campo['funcao'] == "titulo"): ?>
								<tr class="SubtituloTabela" >
									<td class="bold" colspan="2">
										<?php echo $campo['titulo'] ?>
									</td>
								</tr>
							<?php endif; ?>
							<?php if($campo['funcao'] == "hidden"):?>
								<input type="hidden" name="<?php echo $campo['nome'] ?>" id="<?php echo $campo['nome'] ?>" value="<?php echo $campo['value'] ?>"  />
							<?php endif; ?>
						<?php endif; ?>
					<?php endforeach; ?>
				<?php endif; ?>
				<tr class="center SubtituloTabela" >
					<td class="bold" colspan="2">
						<input type="button" name="btn_salvar" value="Salvar" />
						<input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-1)"  />
					</td>
				</tr>
			</table>
		</form>
		<?php
	}
	
	private function validaCPF($cpf)
	{
		
		// Verifiva se o número digitado contém todos os digitos
	    $cpf = str_pad(preg_replace('/[^0-9]/', '', $cpf, -1), 11, '0', STR_PAD_LEFT);
		
		// Verifica se nenhuma das sequências abaixo foi digitada, caso seja, retorna falso
	    if (strlen($cpf) != 11 || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '' || $cpf == '')
		{
			return $this->retornaErroUsuario("CPF inválido.");
	    }
		else
		{   // Calcula os números para verificar se o CPF é verdadeiro
	        for ($t = 9; $t < 11; $t++) {
	            for ($d = 0, $c = 0; $c < $t; $c++) {
	                $d += $cpf{$c} * (($t + 1) - $c);
	            }
	
	            $d = ((10 * $d) % 11) % 10;
	
	            if ($cpf{$c} != $d) {
	                return $this->retornaErroUsuario("CPF inválido.");
	            }
	            
	        }
			
	        return true;
	    }
	}
	
	private function validaCNPJ($cnpj)
    { 
	    	    
    	if (strlen($cnpj) <> 18) return $this->retornaErroUsuario("CNPJ inválido.");; 
	    
	    $soma1 = ($cnpj[0] * 5) + 
	
	    ($cnpj[1] * 4) + 
	    ($cnpj[3] * 3) + 
	    ($cnpj[4] * 2) + 
	    ($cnpj[5] * 9) + 
	    ($cnpj[7] * 8) + 
	    ($cnpj[8] * 7) + 
	    ($cnpj[9] * 6) + 
	    ($cnpj[11] * 5) + 
	    ($cnpj[12] * 4) + 
	    ($cnpj[13] * 3) + 
	    ($cnpj[14] * 2); 
	    $resto = $soma1 % 11; 
	    $digito1 = $resto < 2 ? 0 : 11 - $resto; 
	    $soma2 = ($cnpj[0] * 6) + 
	
	    ($cnpj[1] * 5) + 
	    ($cnpj[3] * 4) + 
	    ($cnpj[4] * 3) + 
	    ($cnpj[5] * 2) + 
	    ($cnpj[7] * 9) + 
	    ($cnpj[8] * 8) + 
	    ($cnpj[9] * 7) + 
	    ($cnpj[11] * 6) + 
	    ($cnpj[12] * 5) + 
	    ($cnpj[13] * 4) + 
	    ($cnpj[14] * 3) + 
	    ($cnpj[16] * 2); 
	    $resto = $soma2 % 11; 
	    $digito2 = $resto < 2 ? 0 : 11 - $resto;
	    
	    if(($cnpj[16] == $digito1) && ($cnpj[17] == $digito2)){
	    	 return true;
	    }else{
	    	return $this->retornaErroUsuario("CNPJ inválido.");
	    }
	}
	
	public function ajaxValidaCNPJ()
    { 
	   	$cnpj = $_REQUEST['cnpj'];
	   	
    	if (strlen($cnpj) <> 18){
    		echo "CNPJ inválido.";
    		return false;
    	} 
	    
	    $soma1 = ($cnpj[0] * 5) + 
	
	    ($cnpj[1] * 4) + 
	    ($cnpj[3] * 3) + 
	    ($cnpj[4] * 2) + 
	    ($cnpj[5] * 9) + 
	    ($cnpj[7] * 8) + 
	    ($cnpj[8] * 7) + 
	    ($cnpj[9] * 6) + 
	    ($cnpj[11] * 5) + 
	    ($cnpj[12] * 4) + 
	    ($cnpj[13] * 3) + 
	    ($cnpj[14] * 2); 
	    $resto = $soma1 % 11; 
	    $digito1 = $resto < 2 ? 0 : 11 - $resto; 
	    $soma2 = ($cnpj[0] * 6) + 
	
	    ($cnpj[1] * 5) + 
	    ($cnpj[3] * 4) + 
	    ($cnpj[4] * 3) + 
	    ($cnpj[5] * 2) + 
	    ($cnpj[7] * 9) + 
	    ($cnpj[8] * 8) + 
	    ($cnpj[9] * 7) + 
	    ($cnpj[11] * 6) + 
	    ($cnpj[12] * 5) + 
	    ($cnpj[13] * 4) + 
	    ($cnpj[14] * 3) + 
	    ($cnpj[16] * 2); 
	    $resto = $soma2 % 11; 
	    $digito2 = $resto < 2 ? 0 : 11 - $resto;
	    
	    if(($cnpj[16] == $digito1) && ($cnpj[17] == $digito2)){
	    	 return true;
	    }else{
	    	echo "CNPJ inválido.";
    		return false;
	    }
	}
	
	public function ajaxCarregaDadosCNPJ()
	{
		$this->CNPJ($_REQUEST['cnpj']);
		$arrDados = $this->recuperaDadosReceita();
		ob_clean();
		header ("content-type: text/xml");
		echo $arrDados;
		exit;
	}
	
	private function retornaErroProgramador($mensagem = null)
	{
		$this->arrErrosProgramador[] = $mensagem;
		dbg($this->arrErrosProgramador);
		return false;
	}
	
	private function retornaErroUsuario($mensagem)
	{
		$this->arrErros[] = $mensagem;
		return false;	
	}
	
	public function recuperaErros()
	{
		return $this->arrErros;	
	}
	
	private function recuperaAtributosColuna()
	{
		if(!$this->table && !$this->schema){
			return false;
		}
		
		$sql = "SELECT DISTINCT
					pg_attribute.attnum as numero,
					pg_attribute.attname AS coluna,
					pg_type.typname as tipo,
					pg_attribute.atttypmod - 4 as tamanho,
					(SELECT description FROM pg_description pd WHERE pg_attribute.attrelid = pd.objoid AND pg_attribute.attnum = pd.objsubid LIMIT 1) AS comentario,
					pg_attribute.attnotnull as naonulo,
					--pg_constraint.*,
					pg_get_constraintdef(pg_constraint.oid) as restricao
				FROM 
					pg_class
				JOIN 
					pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
				JOIN 
					pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
				JOIN
					pg_type ON pg_type.oid = pg_attribute.atttypid
				JOIN 
					pg_index ON pg_index.indrelid=pg_class.oid
				LEFT JOIN
					pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
				WHERE 
					pg_class.relname = '".$this->table."'
				AND
					pg_namespace.nspname = '".$this->schema."'
				AND 
					pg_attribute.attnum > 0
				AND 
					pg_attribute.attrelid = pg_class.oid
				AND 
					pg_attribute.atttypid = pg_type.oid
				".(($arrWhere && is_array($arrWhere)) ? " AND ".implode(" AND ",$arrWhere) : "")."
				ORDER BY
					".(($arrOrder && is_array($arrOrder)) ? implode(",",$arrOrder)."," : "")."
					pg_attribute.attnum";

		$arrDados = $this->carregar($sql);
		
		if($arrDados){
			foreach($arrDados as $dado){
				$arrAttb[$dado['coluna']]['nome'] = $dado['coluna'];
				$arrAttb[$dado['coluna']]['tipo'] = $dado['tipo'];
				$arrAttb[$dado['coluna']]['tamanho'] = $dado['tamanho'];
				$arrAttb[$dado['coluna']]['obrigatorio'] = $dado['naonulo'] == "t" ? "S" : "N";
				if(strstr(strtoupper($dado['restricao']),"FOREIGN KEY")){
					$restricao = $dado['restricao'];
					$restricao = str_replace(array("FOREIGN KEY","REFERENCES"," "),"",$restricao);
					$restricao = substr($restricao,1,strpos($restricao, "ON")-1);
					$fk = substr($restricao,0,strpos($restricao, ")"));
					$tbl = str_replace("$fk)","",substr($restricao,0,strpos($restricao,"(")));
					$fk2 = str_replace( array( "$fk)$tbl(" ,")" ),"",$restricao);
					if(!strstr($tbl,".")){
						$arrAttb[$dado['coluna']]['fk'] = array( "schema" => "public" ,"tabela" => $tbl ,"coluna" => $fk2  );
					}else{
						$arrFK = null;
						$arrFK = explode(".",$tbl);
						$arrAttb[$dado['coluna']]['fk'] = array( "schema" => $arrFK[0] ,"tabela" => $arrFK[1] ,"coluna" => $fk2  );	
					}
				}
			}
		return $arrAttb;
		}
	}
	
	private function montaEndereco($campo,$schema,$tabela)
	{
		?>
		<tr class="SubtituloTabela" >
			<td class="bold esquerda" colspan="2">
				Endereço
				<input type="hidden" name="end_schema" value="<?php echo $schema ?>" />
				<input type="hidden" name="end_table" value="<?php echo $tabela ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				CEP:
			</td>
			<td>
				<?php echo campo_texto("endcep","S","S","",12,10,"##.###-###","","","",""," onchange='RFverificaCEP()' ","",$this->arrEndereco['endcep']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Logradouro:
			</td>
			<td>
				<?php echo campo_texto("endlog","S","S","",60,255,"","","","","","","",$this->arrEndereco['endlog']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Complemento:
			</td>
			<td>
				<?php echo campo_texto("endcom","N","S","",60,255,"","","","","","","",$this->arrEndereco['endcom']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Número:
			</td>
			<td>
				<?php echo campo_texto("endnum","N","S","",40,10,"","","","","","","",$this->arrEndereco['endnum']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Bairro:
			</td>
			<td>
				<?php echo campo_texto("endbai","N","S","",60,255,"","","","","","","",$this->arrEndereco['endbai']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				UF:
			</td>
			<td>
				<?php echo campo_texto("estuf","S","N","",3,2,"","","","","","","",$this->arrEndereco['estuf']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >
				Município:
			</td>
			<td>
				<?php echo campo_texto("mundescricao","S","N","",60,255,"","","","","","","",$this->arrEndereco['mundescricao']) ?>
				<input type="hidden" name="muncod" value="<?php echo $this->arrEndereco['muncod'] ?>" />
			</td>
		</tr>
                
                <script lang="javascript">
                    //Função AJAX que pega os dados do endereço atraves do CEP
                    // as informações são retornadas com o separador "||"
                    jQuery('[name=endcep]').blur(function(){
                        if(jQuery(this).val() != ''){
                            
                        //01503-000
                        jQuery.post( "/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP", {endcep : jQuery(this).val()},function( data ) {
                            var dados = data.split("||");
                                setTimeout(function(){
                                        jQuery('[name=endbai]').val(dados[1]);
                                        jQuery('[name=endlog]').val(dados[0]);
                                        jQuery('[name=estuf]').val(dados[3]);
                                        jQuery('[name=muncod]').val(dados[4]);
                                        jQuery('[name=mundescricao]').val(dados[2]);
                                }, 300);
                          });
                        }
                    });
                </script>
		<?php
	}
	
	public function ajaxRecuperaMunicipio()
	{
		$muncod = $_REQUEST['muncod'];
		$sql = "select mundescricao from territorios.municipio where muncod = '$muncod'";
		echo $this->pegaUm($sql);	
	}
	
	public function salvarDados()
	{
		
		$this->schema($_POST['schema']);
		$this->table($_POST['table']);
		
		$arrDados = $this->recuperaAtributosColuna();
		
		if($arrDados){
			$n = 0;
			foreach($arrDados as $dado){
				if($n == 0){
					$pk = $dado['nome'];
				}else{
					if($this->tipo == "J" && strstr($dado['nome'],"cnpj") && $_POST['cnpj']){
						$arrCampos[]  = $dado['nome'];
						$arrValores[] = "'".str_replace(array(".","/","-"),array("","",""),$_POST['cnpj'])."'";
					}
					if($_POST[$dado['nome']]){
						$arrCampos[]  = $dado['nome'];
						$arrValores[] = "'".$_POST[$dado['nome']]."'";
					}
				}
			$n++;
			}
						
			if($this->tipo == "J"){
				$this->CNPJ($_POST['cnpj']);
				$cnpjcpf = str_replace(array(".","/","-"),array("","",""),$_POST['cnpj']);
			}else{
				$this->CPF($_POST['cpf']);
				$cnpjcpf = str_replace(array(".","/","-"),array("","",""),$_POST['cpf']);
			}
			
			//Verifica se existe a entidade
			$sql = "select entid from entidade.entidade where entnumcpfcnpj = '$cnpjcpf'";
			$entid = $this->pegaUm($sql);
			if(!$entid){
				$entid = $this->salvarEntidade();
			}
			
			$sql = "select $pk from {$_POST['schema']}.{$_POST['table']} where entid = $entid";
			$registro = $this->pegaUm($sql);
			if(!$registro){
				$sql = "insert into {$_POST['schema']}.{$_POST['table']} (entid,".implode(",",$arrCampos).") values ($entid,".implode(",",$arrValores).") returning $pk";
				$valorPK = $this->pegaUm($sql);
				$_SESSION['RF_receita_federal']['msg_form'] = "Dados cadastrados com sucesso.";
			}else{
				$sql = "update {$_POST['schema']}.{$_POST['table']} set ";
				$n=0;
				foreach($arrCampos as $campo){
					$sql.="$campo = ".$arrValores[$n];
					if($n != count($arrCampos)-1){
						$sql.=",";
					}
					$n++;
				}
				$sql.=" where $pk = $registro";
				$this->executar($sql);
				$valorPK = $registro;
				$_SESSION['RF_receita_federal']['msg_form'] = "Dados atualizados com sucesso.";
			}
			
			//Inclui o endereço
			if($_POST['endcep'] && $_POST['end_schema'] && $_POST['end_table']){
				$endcep = str_replace(array(".","-"),array("",""),$_POST['endcep']);
				$endlog = $_POST['endlog'];
				$endcom = $_POST['endcom'];
				$endnum = $_POST['endnum'];
				$endbai = $_POST['endbai'];
				$estuf = $_POST['estuf'];
				$muncod = $_POST['muncod'];
				
				$sql = "select endid from {$_POST['end_schema']}.{$_POST['end_table']} where $pk = $valorPK";
				$endid = $this->pegaUm($sql);
				
				if($endid){
					$sql = "update 
								{$_POST['end_schema']}.{$_POST['end_table']}
							set
								endlog = '$endlog',
								endcep = '$endcep',
								endcom = '$endcom',
								endnum = '$endnum',
								endbai = '$endbai',
								estuf = '$estuf',
								muncod = '$muncod'
							where
								endid = $endid";
					$this->executar($sql);
				}else{
					$sql = "insert into {$_POST['end_schema']}.{$_POST['end_table']} 
							(endlog,endcep,endcom,endnum,endbai,estuf,muncod,endstatus,$pk)
							values
							('$endlog','$endcep','$endcom','$endnum','$endbai','$estuf','$muncod','A',$valorPK)
							returning endid";
					$endid = $this->pegaUm($sql);
					
					//$sql = "update {$_POST['schema']}.{$_POST['table']} set endid = $endid where $pk = $valorPK";
					//$this->executar($sql);
				}
				
			}
			
			$this->commit();
			return $valorPK;
		}
		
	}
	
	function carregarDados($valorPK)
	{
		$arrDados = $this->recuperaAtributosColuna();
		
		if($arrDados){
			$n = 0;
			foreach($arrDados as $dado){
				if($n == 0){
					$pk = $dado['nome'];
					break;
				}
			$n++;
			}
		}
		$sql = "select 
					ent.entnumcpfcnpj, 
					tbl.* 
				from 
					$this->schema.$this->table tbl
				inner join
					entidade.entidade ent ON ent.entid = tbl.entid
				where 
					tbl.$pk = $valorPK";
		$this->arrValores = $this->carregar($sql);
		
		if($this->arrValores[0][$pk] && $this->endereco_schema && $this->endereco_table){
			$sql = "select 
						ende.*,
						mun.mundescricao 
					from 
						{$this->endereco_schema}.{$this->endereco_table} ende
					left join
						territorios.municipio mun ON mun.muncod = ende.muncod
					where
						$pk = {$this->arrValores[0][$pk]}";
			$arrDados = $this->pegaLinha($sql);
			$this->arrEndereco = $arrDados;	
		}
		
	}
	
	public function ajaxCarregaDadosLocal()
	{
		$this->schema($_REQUEST['schema']);
		$this->table($_REQUEST['table']);
		
		if($_REQUEST['end_schema'] && $_REQUEST['end_table']){
			$this->enderecoSchema($_REQUEST['end_schema']);
			$this->enderecoTable($_REQUEST['end_table']);
		}
		
		$attbCampo = $this->recuperaAtributosColuna();
		if($attbCampo){
			$n = 0;
			foreach($attbCampo as $dado){
				if($n == 0){
					$pk = $dado['nome'];
					break;
				}
			$n++;
			}
		}
				
		$cnjp = str_replace(array(".","-","/"),array("","",""),$_REQUEST['cnpj']);
		
		if($this->endereco_schema && $this->endereco_table && $pk){
			$sql = "select 
						ende.* 
					from 
						$this->schema.$this->table tbl
					inner join
						 {$this->endereco_schema}.{$this->endereco_table} ende ON ende.$pk = tbl.$pk
					where 
						entid = (select entid from entidade.entidade where entnumcpfcnpj = '$cnjp')";
			$arrDadosEnd = $this->pegaLinha($sql);
		}
		
		$sql = "select * from $this->schema.$this->table where entid = (select entid from entidade.entidade where entnumcpfcnpj = '$cnjp')";
				
		$arrDados = $this->pegaLinha($sql);
		
		ob_clean();
		header ("content-type: text/xml");
		
		if($arrDados){
			
			$xml = "<DADOS>";
			foreach($arrDados as $campo => $valor){
				$xml.="<$campo>$valor</$campo>";
			}
			if($arrDadosEnd){
				$xml.= "<ENDERECO>";
				foreach($arrDadosEnd as $campo => $valor){
					$xml.="<$campo>$valor</$campo>";
				}
				$xml.= "</ENDERECO>";
			}
			$xml.="</DADOS>";
			
		}else{
			$xml = "<SEMDADOS></SEMDADOS>";
		}
		echo $xml;
		exit;
	}
	
	function salvarEntidade()
	{
		$arrDados = $this->recuperaDadosReceita();
		$arrObj = (object) simplexml_load_string($arrDados);
		
		//Dados da Entidade
		$njuid = (integer) $arrObj->PESSOA->co_natureza_juridica_rf;
		$entnumcpfcnpj = (string) $arrObj->PESSOA->nu_cnpj_rf;
		$entnome = (string) $arrObj->PESSOA->no_empresarial_rf;
		$entstatus = "A";
		$entdatainclusao = "now()";
		
		$sql = "insert into 
					entidade.entidade 
				(njuid,entnumcpfcnpj,entnome,entstatus,entdatainclusao) 
					values
				($njuid,'$entnumcpfcnpj','$entnome','$entstatus',$entdatainclusao) 
				returning 
					entid";
		$entid = $this->pegaUm($sql);
		
		if((string)$arrObj->PESSOA->ENDERECOS->ENDERECO->nu_cep){
			$tpeid = (integer)$arrObj->PESSOA->ENDERECOS->ENDERECO->co_tipo_endereco_pessoa;
			$tpeid = $tpeid == 2 ? 2 : 1;
			$endlog = (string) $arrObj->PESSOA->ENDERECOS->ENDERECO->ds_tipo_logradouro." ".$arrObj->PESSOA->ENDERECOS->ENDERECO->ds_logradouro;
			$endcom = (string) $arrObj->PESSOA->ENDERECOS->ENDERECO->ds_logradouro_comp;
			$endbai = (string) $arrObj->PESSOA->ENDERECOS->ENDERECO->ds_bairro;
			$muncod = (string) str_replace("00000000","",$arrObj->PESSOA->ENDERECOS->ENDERECO->co_cidade);
			$estuf = (string) $arrObj->PESSOA->ENDERECOS->ENDERECO->sg_uf;
			$endnum = (string) $arrObj->PESSOA->ENDERECOS->ENDERECO->ds_numero;
						
			$sql = "select
						endid
					from
						entidade.endereco
					where
						entid = $entid";
			$endid = $this->pegaUm($sql);
			
			if($endid){
				$sql = " update
							entidade.endereco
						set
							tpeid = $tpeid,
							endlog = '$endlog',
							endcom = '$endcom',
							endbai = '$endbai',
							muncod = '$muncod',
							estuf = '$estuf',
							endnum = '$endnum'
						where
							endid = $endid";
			}else{
				$sql = "insert into 
					entidade.endereco 
				(entid,tpeid,endlog,endcom,endbai,muncod,estuf,endnum,endstatus) 
					values
				($entid,$tpeid,'$endlog','$endcom','$endbai','$muncod','$estuf','$endnum','A')";
			}
			$this->executar($sql);
			
		}
		
		$this->commit();
		
		return $entid;
	}
}