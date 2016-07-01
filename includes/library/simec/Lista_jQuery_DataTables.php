<?php

/*
 * Classe listaDT
 * Classe para gerar listas utilizando o Data Tables do jQuery
 * @author Eduardo Dunice Neto
 * @since 10/04/2014
 */
class listaDT {
	
	protected $nome;
	protected $titulo;
	protected $descricaoBusca;
	protected $instrucaoBusca;
	protected $nomeXLS;
	protected $numeroColunasXLS;
	protected $arrDados;
	protected $arrCabecalho;
	protected $arrSemSpan;
	protected $arrCampoNumerico;
	protected $retorno;
	
	public function __construct()
    {
		echo $this->includesCSS();
		echo $this->includesJS();
    }
	
	protected function includesCSS(){
		
		return '
				<link rel="stylesheet" href="../par/css/jquery.dataTables.css" type="text/css" media="all" />
				<link rel="stylesheet" href="../par/css/dataTables.tableTools.css" type="text/css" media="all" />
				<link rel="stylesheet" href="../par/css/jquery.dataTables.min.css" type="text/css" media="all" />
				<link rel="stylesheet" href="../par/css/jquery.dataTables_themeroller.css" type="text/css" media="all" />
				<link rel="stylesheet" href="../par/css/jquery.dataTables.simec-custom.css" type="text/css" media="all" />
				<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>';
	}
	
	protected function includesJS(){
		
		return '
				<script type="text/javascript" src="../includes/jQuery/jquery-migrate-1.2.1.js" ></script>
				<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js" ></script>
				<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js" ></script>		
				<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js" ></script>
				<script type="text/javascript" src="../par/js/jquery.dataTables.js" 	charset="utf8" ></script>
				<script type="text/javascript" src="../par/js/dataTables.tableTools.js"	language="javascript" ></script>
				<script type="text/javascript" src="../par/js/jquery.dataTables.js"		language="javascript" ></script>
				<script>jQuery.noConflict();</script>';
	}

	protected function polularAtributos( $param ){
		
		$this->nome 			= $param['nome'];
		$this->titulo 			= $param['titulo'];
		$this->descricaoBusca 	= $param['descricaoBusca'];
		$this->instrucaoBusca 	= $param['instrucaoBusca'];
		$this->nomeXLS 			= $param['nomeXLS'];
		$this->numeroColunasXLS = $param['numeroColunasXLS'];
		$this->arrDados 		= is_array( $param['arrDados'] ) 			? $param['arrDados'] 			: Array();
		$this->arrCabecalho 	= is_array( $param['arrCabecalho'] ) 		? $param['arrCabecalho'] 		: Array();
		$this->arrSemSpan 		= is_array( $param['arrSemSpan'] ) 			? $param['arrSemSpan'] 			: Array();
		$this->arrCampoNumerico	= is_array( $param['arrCampoNumerico'] ) 	? $param['arrCampoNumerico'] 	: Array();
	}
	
	protected function scriptJS(){
		
		$this->retorno .= '
				<script type="text/javascript">
					jQuery(document).ready(function(){

	 					//######################FUNCAO dataTable() jQuery que monta a tabela dinamicamente através do html#################//
	    				//tabela, busca e paginacao 
	    				jQuery(\'#'.$this->nome.'\').dataTable({
					        "pagingType": "full_numbers",  //mostra a paginação através de números em vez das setas
					        "language": {
		                        "sProcessing":   "Processando...",
		                        "sLengthMenu":   "Mostrar _MENU_ registros",
		                        "sZeroRecords":  "N&atilde;o foram encontrados resultados",
		                        "sInfo":         "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
		                        "sInfoEmpty":    "Mostrando de 0 at&eacute; 0 de 0 registros",
		                        "sInfoFiltered": "(filtrado de _MAX_ registros no total)",
		                        "sInfoPostFix":  "",
		                        "sSearch":       "<span style=\'font-size:16px;margin-left:10px;\'>Buscar:</span>",
		                        "sUrl":          "",
		                        "oPaginate": {
	                                "sFirst":    "Primeiro",
	                                "sPrevious": "Anterior",
	                                "sNext":     "Seguinte",
	                                "sLast":     "&Uacute;ltimo"
		                        }
		                    },//altera o idioma para português
					        "bJQueryUI": false,//habilita o uso do Datatables com o tema do JQueryUI
					        "bFilter": true, //habilitando campo de busca
					        "sDom": \'T<"clear">frtip\',
					        responsive: true,
					        tableTools: {
					            "aButtons": [
			                            {
			                            "sExtends": "xls",
			                            "sButtonText": "<div class=botao style=\'color:#FFFFFF; background-color: #428bca;border: 1px solid #357ebd; padding: 2px 7px;font-size: 9px;\'>Exportar Excel</div>",
			                            "oSelectorOpts": { filter: \'applied\', order: \'current\' },
			                            "sFileName": "'.$this->nomeXLS.'.xls",
			                            "mColumns" :  ['.$this->numeroColunasXLS.'],
			                            "BFooter" : true,
			                            "sInfo": ""
			                            }
	                  				]
	        					}
						});
	     
					    jQuery("#'.$this->nome.'_filter label :input").width(380); //definindo o tamanho do campo Buscar
					    jQuery("#'.$this->nome.'_filter label :input").addClass(\'normal\'); //add class formato padrão simec campo input';
		
		if( $this->instrucaoBusca != '' ){
			$this->retorno .= '
					    jQuery(\'#'.$this->nome.'_filter label\').append(\'<span style="margin-left:10px">'.$this->instrucaoBusca.'</span>\');';
		}
		
		$this->retorno .= '
	    				jQuery("#'.$this->nome.'_filter label :input").on(\'keyup\', function(){
	
	    					var searchTerm = jQuery(this).val().trim().split(" ");
	
							jQuery(\'#'.$this->nome.' tbody tr\').each(function(){
								jQuery(this).children().each(function(){
									
									var html = jQuery(this).find(\'span\').html();
					
									if( html ){
										html = replaceAll(html, \'<b>\',\'\');
										html = replaceAll(html, \'</b>\',\'\');
										jQuery(this).css(\'background-color\',\'\')
							
										if( searchTerm != \'\' ){
											for(var i = 0; i < searchTerm.length; i++) {
												var pattern = "([^\\w]*)(" + searchTerm[i] + ")([^\\w]*)";
												var rg = new RegExp(pattern);
												var match = rg.exec(html);
												
												if(match) {
													jQuery(this).css(\'background-color\',\'#DDFFDD\');
													html = html.replace(rg,match[1] + "<b>"+ match[2] +"</b>" + match[3]);
												}
											}
										}
										jQuery(this).children(\'span\').html(html);
									}
								});
							});
					    });
					});
				</script>';
	}
	
    public function lista( $param )
    {
    	$this->retorno = '';
		$this->polularAtributos( $param );
        $this->scriptJS();
        $this->montaCorpoLista();
        return $this->retorno;
    }
	
    protected function montaCorpoLista( )
    {
        $this->retorno .= '
        				<center>
        				<div class="conteudo_tabelas">';
        if( $this->titulo != '' ){
	        $this->retorno .= '
						    <div class="tabela titulo_tabelas" style="margin-left: 7px; margin-right: 5px; width: 97.5% !important;">
						        <span class="TituloTela" style="margin-left: 604px; float: left;">'.$this->titulo.'</span>
						    </div>';
        }
        $this->retorno .= '
						    <br><br><br>
						    <table id="'.$this->nome.'" class="tabela display listagem" cellspacing="0" width="95%">
						        <Thead>
						            <tr>';
        foreach( $this->arrCabecalho as $cabecalho ){
        	$this->retorno .= "<th class=td_cabeca_lista >$cabecalho &nbsp;&nbsp;</th>";
        }

        $this->retorno .= '			</tr>
						        </Thead>
						        <Tbody>';
		foreach( $this->arrDados as $dados ){
			
        	$this->retorno .= "<tr class=tr_corpo_lista > ";
        	
        	$k = 0;
			foreach( $dados as $dado ){

				if( $this->arrCampoNumerico[$k] == 'S' ){
					$dado = 'R$ '.number_format($dado,2,'.',',');
				}

				if( $this->arrSemSpan[$k] != 'N' ){
					$dado = "<span>$dado</span>";
				}
				
	        	$this->retorno .= "<td>$dado</td>";

	        	$k++;
			}
			
        	$this->retorno .= "</tr>";
		}
		
		$this->retorno .= '		</Tbody>
						    </table>
							<br>
						</div>
						</center>';
    }
}
