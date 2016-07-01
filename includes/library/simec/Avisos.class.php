<?php


class Avisos{
	
	public function __construct(){
	}
	
	/*
	 * Parametros do array:
	 * array(
	 * 		array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
	 *		array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
	 *		..................
	 * )
	 * 
	 * */
	function getAvisos($arrAvisos = array(), $adm = false )
	{
		$arrayCores = array('#FF6D3F', '#FF9B00', '#FFDA00', '#95D641', '#1CE8B5','#3FC3FF', '#B8C4C9' );
		
		if (count($arrAvisos) > 0)
		{
			$strRetorno = "<div id=\"dialog_vigencia\" style=\"display: none;\">";
			$strRetorno.= "<h4>Prezado(a),</h4>";
			$strRetorno.= '<p style="margin: 10px 0px;">Os quadros abaixo apresentam as obras com problema no sistema.</p>';
			$strRetorno.= '<div style="clear: both"></div>';
			
			if (count($arrAvisos) == 1)
			{
				$size = 'large';
			}
			
			if (count($arrAvisos) == 2)
			{
				$size = 'medium';
			}
			
			if (count($arrAvisos) == 3)
			{
				$size = 'small';
			}
			
			foreach($arrAvisos as $k => $aviso)
			{
				$class = 'box-black';
				
				if (isset($aviso['status'])) {
					if ($aviso['status'] == E_ERROR) {
						$class = 'box-red';
					} else if ($aviso['status'] == E_WARNING || $aviso['status'] == E_NOTICE) {
						$class = 'box-orange';
					} else {
						$class = 'box-green';
					}
				}
				
				$strRetorno .= '<div class="box ' . $class . ' box-' . $size . '">';
				$strRetorno .= '	<div class="box-header">' . strtoupper($aviso['titulo']) . '</div>';
				$strRetorno .= '	<div class="box-body">';
				$strRetorno .= '		<p class="box-body-subtitle"></p>';
				$strRetorno .= '		<p>' . $aviso['texto'] . '</p>';
				$strRetorno .= '	</div>';
				$strRetorno .= '	<div class="box-footer"></div>';
				$strRetorno .= '</div>';
			}
			
			$strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
			$strRetorno .= "Equipe PAR MEC/FNDE</p>";
			$strRetorno .= "</div>";
			
			$strRetorno .= "
				<style>
		            .box.box-small {
		                width: 30.3%;
		            }
		
		            .box.box-medium {
		                width: 47%;
		            }
		
		            .box.box-large {
		                width: 97%;
		            }
		
		            .box {
		                FONT: 11pt Arial;
		                -moz-border-radius: 20px;
		                border-radius: 20px;
		                padding: 10px;
		                margin: 10px;
		                float: left;
		            }
		
		            .box .box-header {
		                text-align: center;
		                color: #FFFFFF;
		                height: 30px;
		                font-weight: bold;
		                font-size: 14px;
		            }
		
		            .box .box-header .box-header-options{
		                cursor: pointer;
		                float: right;
		                margin: 0 8px 0 0;
		            }
		
		            .box .box-body {
						text-indent: 10px;
		                text-align: left;
		                background-color: #FFFFFF;
		                border-radius: 20px;
		                border-radius: 5px;
		                padding: 4px;
		                min-height: 130px;
		            }
		            .box .box-body .box-body-title {
		                font-weight: bold;
		                font-size: 14px;
		            }
		            .box .box-body .box-body-subtitle {
		                font-size: 11px;
		            }
		
		            .box.box-red {
		                background-color: #EE3B3B;
		            }
		
		            .box.box-black {
		                background-color: #000000;
		            }
		
		            .box.box-yellow {
		                background-color: #FFC200;
		            }
		
		            .box.box-green {
		                background-color: #348300;
		            }
		
		            .box.box-purple {
		                background-color: #6900AF;
		            }
		
		            .box.box-blue {
		                background-color: #3871C8;
		            }
		
		            .box.box-orange {
		                background-color: #FF8500;
		            }
		            .box p {
		                margin: 0;
		                padding: 0;
		            } 
		            .print{
		                background-color: #FFF;
		                padding: 1px;
		                border-bottom: 1px solid #000;
		                border-right: 1px solid #000;
		                border-top: 1px solid #CCC;
		                border-left: 1px solid #CCC;
		            }
				</style>";
			
			if(! $adm)
			{
				$strRetorno .=	"			
				 	<script type=\"text/javascript\">
				        jQuery(function(){
		                    jQuery(\"#dialog_vigencia\").dialog({
		                        modal: true,
		                        width: '90%',
								height: 700,
		                        open: function(){
						            jQuery('.ui-widget-overlay').bind('click',function(){
						                jQuery('#dialog_vigencia').dialog('close');
						            })
						        },
		                        buttons: {
		                            Fechar: function() {
		                                jQuery(\"#dialog_detalhe_processo\").html('');
		                                jQuery( this ).dialog( \"close\" );
		                            }
		                        }
		                    });
				        });
				    </script>";
			}
                        
		echo $strRetorno;
		}
	}
}